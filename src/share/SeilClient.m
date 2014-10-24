#import "SeilClient.h"
#import "SeilKeys.h"

@implementation SeilClient

- (id<SeilProtocol>)proxy {
  @synchronized(self) {
    if (!proxy_) {
      proxy_ = [NSConnection rootProxyForConnectionWithRegisteredName:kSeilConnectionName host:nil];
      [proxy_ setProtocolForProxy:@protocol(SeilProtocol)];
    }
    return proxy_;
  }
}

- (void)observer_NSConnectionDidDieNotification:(NSNotification *)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      NSLog(@"observer_NSConnectionDidDieNotification is called");
      proxy_ = nil;
    };
  });
}

- (id)init {
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSConnectionDidDieNotification:)
                                                 name:NSConnectionDidDieNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  // Call removeObserver first because observer may refresh connection.
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
