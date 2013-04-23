//
//  DLDownload.h
//
//  Copyright 2013 Ender Labs. All rights reserved.
//  Created by Donald Hays.
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

typedef void (^DownloadDidUpdateProgressCallback)(NSUInteger bytesReceived, NSUInteger expectedLength, CGFloat percent);
typedef void (^DownloadDidFinishCallback)(NSData *data, NSError *error);

extern NSString *DownloadDidBeginNotification;
extern NSString *DownloadDidEndNotification;

@interface DLDownload : NSObject
@property (nonatomic, copy) DownloadDidFinishCallback callback;
@property (nonatomic, copy) DownloadDidUpdateProgressCallback updateProgressCallback;
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
