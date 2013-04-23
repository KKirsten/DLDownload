# DLDownload
DLDownload is a lightweight block based wrapper around NSURLConnection.

## Installation
* Add `pod "DLDownload"` to your [Podfile](http://cocoapods.org), import as necessary with: `#import "DLDownload.h"`.
* Add `DLDownload.{h,m}` to your project, import as necessary with: `#import "DLDownload.h"`.

## Documentation
Documentation for DLDownload can be found on [Github Pages](http://enderlabs.github.io/DLDownload/).

## Contributing
Please feel free to [fork the code](http://github.com/enderlabs/DLDownload.git), and submit pull requests for any new features or fixes.

## Usage


### Simple HTTP Call
 
    DLDownload *download = [[DLDownload alloc] init];
    download.url = [NSURL URLWithString:@"http://track8.fm/api/track"];
    download.parameters = @{@"short_code":@"rA"};
    download.callback =  ^(NSData *data, NSError *error) {
        if(error) {
            // handle error.
        } else {
            NSDictionary *jsonData = [NSJSONSerialization JSONObjectWithData:data options:0 error:0];
            NSDictionary *trackData = [jsonData objectForKey:@"track"];
            NSLog(@"%@", [trackData objectForKey:@"name"]);
        }  
    };

    [download start];


### HTTP Methods

HTTP methods supported include: `GET`, `POST`, `DELETE`, `PUT`, `PATCH`.

    DLDownload *download = [[DLDownload alloc] init];
    download.method = DLDownloadMethodPOST;
    download.url = [NSURL URLWithString:@"http://track8.fm/api/track"];
    download.parameters = @{@"artist":@"Finch", @"track":@"Ender"};
    download.callback =  ^(NSData *data, NSError *error) {
        if(error) {
            // handle error.
        } else {
            NSDictionary *jsonData = [NSJSONSerialization JSONObjectWithData:data options:0 error:0];
            NSDictionary *trackData = [jsonData objectForKey:@"track"];
            NSLog(@"%@ : %@", [trackData objectForKey:@"name"], [trackData objectForKey:@"linkURL"]);
        }
    };

    [download start];


### Monitor Download Progress

    DLDownload *download = [[DLDownload alloc] init];
    download.method = DLDownloadMethodPOST;
    download.url = [NSURL URLWithString:@"http://imgsrc.hubblesite.org/hu/db/images/hs-2006-10-a-hires_jpg.jpg"];
    download.updateProgressCallback = ^(NSUInteger bytesReceived, NSUInteger expectedLength, CGFloat percent) {
        NSLog(@"%i%% Complete", (int)(percent * 100));
    };
    download.callback =  ^(NSData *data, NSError *error) {
        if(error) {
            // handle error.
        } else {
            UIImage *hubbleImage = [UIImage imageWithData:data];
            self.someImageView.image = hubbleImage;
        }
    };

    [download start];

### HTTP Headers

    NSString *authorizationHeader = [NSString stringWithFormat:@"Bearer %@", @"UserServiceAccessToken"];
    NSMutableDictionary *headerFields = @{@"Authorization":authorizationHeader, @"content-type": @"application/json" };

    DLDownload *download = [[DLDownload alloc] init];
    download.HTTPHeaderFields = headerFields;

    // define properties and methods here.

    [download start];


### HTTP Body

    DLDownload *download = [[DLDownload alloc] init];
    download.bodyData = [NSJSONSerialization dataWithJSONObject:@{@"text":@"Message to post to a social network."} options:0 error:0];

    // define properties and methods here.

    [download start];

### HTTP Authentication Challenge

    DLDownload *download = [[DLDownload alloc] init];
    download.credential = [NSURLCredential credentialWithUser:@"username" password:@"password" persistence:NSURLCredentialPersistenceForSession];

    // define properties and methods here.

    [download start];


## Notifications
* `DLDownloadDidBeginNotification`
* `DLDownloadDidEndNotification`

## Notes
You do not need to encode parameters. All parameters are properly encoded by DLDownload before the call is made.