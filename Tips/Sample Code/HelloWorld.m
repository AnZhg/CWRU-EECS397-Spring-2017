// Created by AnZhg on Jan. 3, 2017.
// Case Western Reserve University.
// This is manual retain release version.
// Compile: gcc -Wall -Wextra HelloWorld.m `gnustep-config --objc-flags` `gnustep-config --base-libs` -o HelloWorld_objc
// Run: ./HelloWorld_objc

#import <Foundation/Foundation.h>

// Objective-C class interface.
@interface AZGHelloWorld : NSObject
{
    __strong NSString *string;
}

@property (retain, nonatomic) NSString *string;

- (NSString *)retrieveHello;
- (void)printStringAfter:(NSString *)str;

@end

// Objective-C class implementation.
@implementation AZGHelloWorld

@synthesize string;

- (instancetype)init
{
    self = [super init];
    
    if (self != nil) {
        string = [[NSString alloc] initWithString:@"World!"];
    }
    
    return self;
}
-Wall -Wextra HelloWorld.m `gnustep-config --objc-flags` `gnustep-config --base-libs` -o HelloWorld_objc
- (void)dealloc
{
    [string release];
    [super dealloc];
}

- (void)setString:(NSString *)value
{
    // Overwite the default setter method.
    if ([string isEqualToString:value] == NO) {
        [string release];
        string = [value retain];
    }
}

- (NSString *)retrieveHello
{
    return [[[NSString alloc] initWithString:@"Hello"] autorelease];
}

- (void)printStringAfter:(NSString *)str
{
    NSLog(@"%@ %@", str, self.string);
}

@end

// main.
int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    AZGHelloWorld *helloWorld = [[AZGHelloWorld alloc] init];
    [helloWorld printStringAfter:[helloWorld retrieveHello]];
    [helloWorld release];
    
    [pool drain];
    
    return 0;
}
