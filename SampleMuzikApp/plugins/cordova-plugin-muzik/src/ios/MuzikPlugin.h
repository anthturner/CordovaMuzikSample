//
//  MuzikPlugin.h
//
//
//  Created by Michael Spearman on 2/8/16.
//
//

#import <Cordova/CDV.h>
#import "MzAccessory.h"

extern NSString* const ACCELEROMETER_DATA;
extern NSString* const GESTURE_ACTION_KEY;
extern NSString* const GESTURE_RECEIVED;

@interface MuzikPlugin : CDVPlugin <MzAccessoryDelegate>

@property (nonatomic, strong) MzAccessory* mza;
@property (nonatomic, strong) NSMutableDictionary* callbackIds;

//Actions
- (void) RequestAccelerometerData:(CDVInvokedUrlCommand*)command;
- (void) SetGestureActionKey:(CDVInvokedUrlCommand*)command;
- (void) CheckIsAuthorized:(CDVInvokedUrlCommand*)command;
- (void) RequestIsDevAuthorized:(CDVInvokedUrlCommand*)command;
- (void) RequestBatteryLevelPercent:(CDVInvokedUrlCommand*)command;
- (void) RequestDeviceMetadata:(CDVInvokedUrlCommand*)command;
- (void) SetAccelerometerDataReceiveBuffer:(CDVInvokedUrlCommand*)command;
- (void) RequestTrackInfo:(CDVInvokedUrlCommand*)command;

//Callbacks
- (void) RegisterGestureReceived:(CDVInvokedUrlCommand*)command;
//- (void) RegisterWasAttached:(CDVInvokedUrlCommand*)command;


//Callback Id storing/loading
- (void) StoreCallbackId:(NSString*)callbackId forKey:(NSString*)key;
- (NSString*) GetCallbackId:(NSString*)key;

@end
