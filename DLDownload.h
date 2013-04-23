//
//  DLDownload.h
//
//  Copyright 2013 Ender Labs. All rights reserved.
//  Created by Donald Hays.
//

#import <Foundation/Foundation.h>

#define kDownloadStandardTimeout 10.0

/**
 DLDownload is a lightweight wrapper around NSURLConnection.
 */

typedef enum {
    DLDownloadMethodGET = 0,
    DLDownloadMethodPOST = 1,
    DLDownloadMethodDELETE = 2,
    DLDownloadMethodPUT = 3,
    DLDownloadMethodPATCH = 4,
} DLDownloadMethod;

typedef void (^DLDownloadDidUpdateProgressCallback)(NSUInteger bytesReceived, NSUInteger expectedLength, CGFloat percent);
typedef void (^DLDownloadDidFinishCallback)(NSData *data, NSError *error);

extern NSString *DLDownloadDidBeginNotification;
extern NSString *DLDownloadDidEndNotification;

@interface DLDownload : NSObject

/**---------------------------------------------------------------------------------------
 * @name Request Callbacks
 *  ---------------------------------------------------------------------------------------
 */

/** A block that is called when a download has finished. */
@property (nonatomic, copy) DLDownloadDidFinishCallback callback;

/**  A block that is called when the progress of a download has updated. */
@property (nonatomic, copy) DLDownloadDidUpdateProgressCallback updateProgressCallback;

 /**---------------------------------------------------------------------------------------
 * @name Request Properties
 *  ---------------------------------------------------------------------------------------
 */

 /** The request's HTTP request method. 

The default method is GET.
*/
@property (nonatomic) DLDownloadMethod method;

/** Time in seconds until the request should timeout. 

The default timeout is set to 10 seconds.
*/
@property (nonatomic) NSTimeInterval timeout;

/** The request's URL. */
@property (nonatomic, copy) NSURL *url;

/** The parameters to be sent with a request. */
@property (nonatomic, copy) NSDictionary *parameters;

/** A dictionary containing a request's HTTP header fields. */
@property (nonatomic, copy) NSDictionary *HTTPHeaderFields;

/** This data is sent as the message body of a request, as in an HTTP POST request. */
@property (nonatomic, copy) NSData *bodyData;

/** An NSURLCredential object for authenticating a request. */
@property (nonatomic, copy) NSURLCredential *credential;

/**---------------------------------------------------------------------------------------
 * @name Creating / Canceling Requests
 *  ---------------------------------------------------------------------------------------
 */
/**
Starts the download for a given request.
 */
- (void)start;

/**
Cancels the download for a given URL.
 */
- (void)cancel;
@end