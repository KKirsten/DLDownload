# Download
Block based downloading.

## Installation
* Add `pod "Download"` to your [Podfile](http://cocoapods.org), import as necessary with: `#import Download.h`.
* Add `Download.{h,m}` to your project, import as necessary with: `#import "Download.h"`.

## Usage


### Simple HTTP Call

```objc  
Download *download = [[Download alloc] init];
download.url = [NSURL URLWithString:@"http://track8.fm/api/track"];
download.parameters = @{@"short_code":@"rA"};
download.callback =  ^(NSData *data, NSError *error) {
    if(!error) {
        NSDictionary *jsonData = [NSJSONSerialization JSONObjectWithData:data options:0 error:0];
        NSDictionary *trackData = [jsonData objectForKey:@"track"];
        NSLog(@"%@", [trackData objectForKey:@"name"]); // returns "Ender"
    } else {
        // handle error.
    }  
};

[download start];

```

### Define HTTP Method

HTTP methods supported include: `GET`, `POST`, `DELETE`, `PUT`, `PATCH`.

```objc  
Download *download = [[Download alloc] init];
download.method = DownloadMethodPOST;
download.url = [NSURL URLWithString:@"http://track8.fm/api/track"];
download.parameters = @{@"artist":@"Finch",@"track":@"Ender"};
download.callback =  ^(NSData *data, NSError *error) {
    if(!error) {
        NSDictionary *jsonData = [NSJSONSerialization JSONObjectWithData:data options:0 error:0];
        NSDictionary *trackData = [jsonData objectForKey:@"track"];
        NSLog(@"%@ : %@", [trackData objectForKey:@"name"], [trackData objectForKey:@"linkURL"]); // returns "Ender"
    } else {
        // handle error.
    }
};

[download start];
```

### Monitor Download Progress

```objc
Download *download = [[Download alloc] init];
download.method = DownloadMethodPOST;
download.url = [NSURL URLWithString:@"http://imgsrc.hubblesite.org/hu/db/images/hs-2006-10-a-hires_jpg.jpg"];
download.updateProgressCallback = ^(NSUInteger bytesReceived, NSUInteger expectedLength, CGFloat percent) {
    NSNumber *percentDownloaded =  [NSNumber numberWithFloat: percent * 100];
    NSLog(@"%i%% Complete", [percentDownloaded intValue]);
};
download.callback =  ^(NSData *data, NSError *error) {
    if(!error) {
        UIImage *hubbleImage = [UIImage imageWithData:data];
        self.someImageView.image = hubbleImage;
    } else {
        // handle error.
    }
};

[download start];
```

### HTTP Headers

```objc
NSString *authorizationHeader = [NSString stringWithFormat:@"Bearer %@", @"UserServiceAccessToken"];
NSMutableDictionary *headerFields = @{'Authorization':authorizationHeader, @"content-type": @"application/json" };

Download *download = [[Download alloc] init];
download.HTTPHeaderFields = headerFields;

// skipping additional download setup addressed above.

[download start];
```

### HTTP Body

```objc
Download *download = [[Download alloc] init];
download.bodyData = [NSJSONSerialization dataWithJSONObject:@{@"text":@"Message to post to a social network."} options:0 error:0];

// skipping additional download setup addressed above.

[download start];
```

### HTTP Authentication Challenge

```objc
Download *download = [[Download alloc] init];
download.challengeCredential = [[NSURLCredential alloc] initWithUser:@"username" password:@"password" objectForKey:@"CloudAppPassword"] persistence:NSURLCredentialPersistenceNone];
// skipping additional download setup addressed above.

[download start];
```

## Notifications
* `DownloadDidBeginNotification`
* `DownloadDidEndNotification`

## Notes
You do not need to encode parameters. All parameters are properly encoded by Download before the call is made.

## Contributing
Please feel free to fork this repo and submit pull requests for any new features or fixes.