//
//  DLDownload.h
//
//  Copyright 2013 Ender Labs. All rights reserved.
//  Created by Donald Hays.
//

#import <Foundation/Foundation.h>

#define kDLDownloadStandardTimeout 10.0

/**
 * DLDownload is a lightweight block-based wrapper around NSURLConnection. It's
 * optimized for convenience, allowing you to instantiate, configure, and begin
 * a download, as well as write the download completion handler, all from one
 * place in your code. Additionally, instances of DLDownload are "fire and
 * forget", you do not need to keep a reference to the instance alive in an
 * owning object, unless there's the possibility that you will want to cancel
 * the download explicitly.
 *
 * At minimum, you must set the `url` and `callback` properties before calling
 * `start`. Those parameters are enough to do a basic HTTP GET call. Additional
 * parameters can be set to further customize the call.
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

/**
 * -----------------------------------------------------------------------------
 * @name Request Callbacks
 * -----------------------------------------------------------------------------
 */

/**
 * A block that is called when a download has finished.
 * 
 * The block must take two parameters, an `NSData` and an `NSError`. If the
 * download succeeds, the `NSData` parameter will be non-nil and will contain
 * the response data from the server. If the download fails, the `NSData`
 * parameter will be nil, and the `NSError` parameter will be a non-nil instance
 * and will describe the error that occurred. This callback will not be invoked
 * if the download is manually cancelled.
 *
 * This block is required. You must provide it before invoking `start`,
 * otherwise an exception will be raised when the download finishes.
 */
@property (nonatomic, copy) DLDownloadDidFinishCallback callback;

/**
 * A block that is called when the progress of a download has updated.
 * 
 * The block must take three parameters, two `NSUInteger`s and a `CGFloat`. The
 * first `NSUInteger` will be the number of bytes that have been downloaded. The
 * second will be the number of bytes that the download expects to receive. The
 * `CGFloat` will be a percent value (0.0 - 1.0) corresponding to the percent of
 * bytes downloaded.
 *
 * This block is optional.
 */
@property (nonatomic, copy) DLDownloadDidUpdateProgressCallback updateProgressCallback;

/**
 * -----------------------------------------------------------------------------
 * @name Request Properties
 * -----------------------------------------------------------------------------
 */

/**
 * The request's HTTP method.
 *
 * The default value is DLDownloadMethodGET.
 */
@property (nonatomic) DLDownloadMethod method;

/**
 * Time in seconds until the request should timeout.
 *
 * The default value is 10 seconds.
 */
@property (nonatomic) NSTimeInterval timeout;

/**
 * The request's URL.
 *
 * This value must not be nil when `start` is called.
 */
@property (nonatomic, copy) NSURL *url;

/**
 * The parameters to be sent with a request.
 *
 * The handling of this property depends on the download method. For example, in
 * both GET and POST calls, the parameters will be translated to a query string
 * (with dictionary keys listed as query string field names, and dictionary
 * values as query string field values). In a GET call, however, the query
 * string will be appending to the URL, while in a POST call it'll be sent as
 * the request's body data.
 */
@property (nonatomic, copy) NSDictionary *parameters;

/**
 * A dictionary containing HTTP header fields.
 */
@property (nonatomic, copy) NSDictionary *HTTPHeaderFields;

/**
 * This data is sent as the message body of a request, as in an HTTP POST
 * request.
 *
 * If this value is non-nil, it will be sent as the body in POST, DELETE, PUT,
 * and PATCH requests instead of the contents of the `parameters` dictionary.
 */
@property (nonatomic, copy) NSData *bodyData;

/** An NSURLCredential object for authenticating a request. */
@property (nonatomic, copy) NSURLCredential *credential;

/**
 * -----------------------------------------------------------------------------
 * @name Creating / Canceling Requests
 * -----------------------------------------------------------------------------
 */

/**
 * Starts the download.
 *
 * @exception NSGenericException if `start` has already been called on the
 * receiver. DLDownload instances are not reusable; they should be executed once
 * and then discarded.
 * @exception NSInvalidArgumentException if the value of the `url` property is
 * nil.
 */
- (void)start;

/**
 * Cancels the download.
 *
 * A Cancelled download will not invoke its `callback` block.
 */
- (void)cancel;
@end