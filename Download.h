//
//  Download.h
//
//  Created by Donald Hays on 1/17/13.
//
//

#import <Foundation/Foundation.h>

#define kDownloadStandardTimeout 10.0

typedef enum {
    DownloadMethodGET = 0,
    DownloadMethodPOST = 1,
    DownloadMethodDELETE = 2,
    DownloadMethodPUT = 3,
    DownloadMethodPATCH = 4,
} DownloadMethod;

typedef void (^DownloadDidFinishCallback)(NSData *data, NSError *error);

@interface Download : NSObject
@property (nonatomic, copy) DownloadDidFinishCallback callback;
@property (nonatomic) DownloadMethod method;
@property (nonatomic) NSTimeInterval timeout;
@property (nonatomic, copy) NSURL *url;
@property (nonatomic, copy) NSDictionary *parameters;
@property (nonatomic, copy) NSDictionary *HTTPHeaderFields;
@property (nonatomic, copy) NSData *bodyData;
@property (nonatomic, copy) NSURLCredential *credential;

- (void)start;
- (void)cancel;
@end
