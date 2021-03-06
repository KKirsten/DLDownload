//
//  DLDownload.m
//
//  Copyright 2013 Ender Labs. All rights reserved.
//  Created by Donald Hays.
//

#import "DLDownload.h"

NSString *DLDownloadDidBeginNotification = @"DLDownloadDidBeginNotification";
NSString *DLDownloadDidEndNotification = @"DLDownloadDidEndNotification";

@interface DLDownload() <NSURLConnectionDelegate>
@property (nonatomic) long long expectedContentLength;
@property (nonatomic, strong) NSURLConnection *connection;
@property (nonatomic, strong) NSMutableData *data;

- (NSString *)queryStringFromParameters;
@end

@implementation DLDownload
#pragma mark -
#pragma mark Lifecycle
- (id)init
{
    self = [super init];
    if(self) {
        self.method = DLDownloadMethodGET;
        self.timeout = kDLDownloadStandardTimeout;
    }
    return self;
}

- (void)dealloc
{
    [self cancel];
    self.connection = nil;
}

#pragma mark -
#pragma mark Private API
- (NSString *)queryStringFromParameters
{
    NSMutableArray *pairs = [[NSMutableArray alloc] init];
    
    for(NSString *key in self.parameters) {
        NSString *value = [self.parameters objectForKey:key];
        if(![value isKindOfClass:[NSString class]]) {
            value = [value description];
        }
        
        CFStringRef encodedKey = CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)key, NULL, CFSTR("!*'\"();:@&=+$,/?%#[]% "), kCFStringEncodingUTF8);
        CFStringRef encodedValue = CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)value, NULL, CFSTR("!*'\"();:@&=+$,/?%#[]% "), kCFStringEncodingUTF8);
        
        NSString *pair = [NSString stringWithFormat:@"%@=%@", (__bridge NSString *)encodedKey, (__bridge NSString *)encodedValue];
        
        CFRelease(encodedKey);
        CFRelease(encodedValue);
        
        [pairs addObject:pair];
    }
    
    if(pairs.count == 0) {
        return @"";
    }
    
    return [pairs componentsJoinedByString:@"&"];
}

#pragma mark -
#pragma mark Public API
- (void)start
{
    if(self.connection) {
        [NSException raise:NSGenericException format:@"Cannot start a download that is already in progress."];
    }
    
    if(self.url == nil) {
        [NSException raise:NSInvalidArgumentException format:@"Cannot start a download without a URL."];
    }
    
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:self.url cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:self.timeout];
    NSString *queryString = [self queryStringFromParameters];
    NSData *queryData = [queryString dataUsingEncoding:NSUTF8StringEncoding];
    
    if(self.method == DLDownloadMethodGET) {
        [request setHTTPMethod:@"GET"];
        if(queryString.length > 0) {
            if([self.url.absoluteString rangeOfString:@"?"].location == NSNotFound) {
                request.URL = [NSURL URLWithString:[NSString stringWithFormat:@"%@?%@", self.url.absoluteString, queryString]];
            } else {
                request.URL = [NSURL URLWithString:[NSString stringWithFormat:@"%@?%@", self.url.absoluteString, queryString]];
            }
        }
    } else {
        if(self.method == DLDownloadMethodPOST) {
            [request setHTTPMethod:@"POST"];
        } else if(self.method == DLDownloadMethodDELETE) {
            [request setHTTPMethod:@"DELETE"];
        } else if(self.method == DLDownloadMethodPUT) {
            [request setHTTPMethod:@"PUT"];
        } else if(self.method == DLDownloadMethodPATCH) {
            [request setHTTPMethod:@"PATCH"];
        } else {
            [NSException raise:NSGenericException format:@"Unknown download method %i", self.method];
        }
        
        [request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"content-type"];
        if(self.bodyData) {
            [request setHTTPBody:self.bodyData];
        } else {
            [request setHTTPBody:queryData];
        }
    }
    
    for(NSString *HTTPHeaderField in self.HTTPHeaderFields) {
        [request setValue:[self.HTTPHeaderFields objectForKey:HTTPHeaderField] forHTTPHeaderField:HTTPHeaderField];
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName:DLDownloadDidBeginNotification object:self];
    
    self.data = [[NSMutableData alloc] init];
    self.connection = [[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:NO];
    [self.connection scheduleInRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
    [self.connection start];
}

- (void)cancel
{
    [[NSNotificationCenter defaultCenter] postNotificationName:DLDownloadDidEndNotification object:self];
    
    self.callback = nil;
    [self.connection cancel];
    self.connection = nil;
}

#pragma mark -
#pragma mark NSURLConnectionDelegate
- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
{
    if([challenge previousFailureCount] == 0 && self.credential != nil) {
        [[challenge sender] useCredential:self.credential forAuthenticationChallenge:challenge];
    } else {
        self.callback(nil, [NSError errorWithDomain:@"DLDownloadErrorDomain" code:0 userInfo:@{
                         NSLocalizedDescriptionKey : @"Authentication Failed",
                  NSLocalizedFailureReasonErrorKey : @"No authentication credential was provided."
        }]);
        
        [self cancel];
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    self.expectedContentLength = response.expectedContentLength;
    [self.data setLength:0];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [self.data appendData:data];
    
    if(self.expectedContentLength != NSURLResponseUnknownLength && self.expectedContentLength != 0) {
        if(self.updateProgressCallback) {
            self.updateProgressCallback(self.data.length, self.expectedContentLength, (CGFloat)self.data.length / (CGFloat)self.expectedContentLength);
        }
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    [[NSNotificationCenter defaultCenter] postNotificationName:DLDownloadDidEndNotification object:self];
    
    self.connection = nil;
    
    [self.data setLength:0];
    self.callback(nil, error);
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    [[NSNotificationCenter defaultCenter] postNotificationName:DLDownloadDidEndNotification object:self];
    
    self.connection = nil;
    
    if(self.expectedContentLength != NSURLResponseUnknownLength && self.expectedContentLength != 0) {
        if(self.updateProgressCallback) {
            self.updateProgressCallback(self.data.length, self.expectedContentLength, (CGFloat)self.data.length / (CGFloat)self.expectedContentLength);
        }
    }
    
    self.callback(self.data, nil);
}
@end
