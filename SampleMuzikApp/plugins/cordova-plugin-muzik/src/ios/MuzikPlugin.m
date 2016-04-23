//
//  MuzikPlugin.m
//
//
//  Created by Michael Spearman on 2/8/16.
//
//

#import "MuzikPlugin.h"

NSString* const ACCELEROMETER_DATA = @"ACCELEROMETER_DATA";
NSString* const GESTURE_ACTION_KEY = @"GESTURE_ACTION_KEY";
NSString* const GESTURE_RECEIVED = @"GESTURE_RECEIVED";
NSString* const DEV_AUTHORIZED = @"DEV_AUTHORIZED";
NSString* const TRACK_INFO = @"TRACK_INFO";
NSString* const DEVICE_METADATA = @"DEVICE_METADATA";
NSString* const BATTERY_LEVEL = @"BATTERY_LEVEL";

@implementation MuzikPlugin

- (void) pluginInitialize
{
    self.mza = [[MzAccessory alloc] init];
    self.callbackIds = [[NSMutableDictionary alloc] init];
    
    [self.mza authorizeDevWithKey:@"ca44a33a-ab40-11e4-a496-2fb859c5653e"];
    [self.mza addUpdateDelegate:self];
    [self.mza initializeCallbackEngine];
}

/* 
 
 CallbackId storing & loading
 
 */

- (void) StoreCallbackId:(NSString*)callbackId forKey:(NSString*)key
{
    if (![self.callbackIds objectForKey:key])
    {
        [self.callbackIds setObject:callbackId forKey:key];
    }
}

- (NSString*) GetCallbackId:(NSString*)key
{
    return [self.callbackIds objectForKey:key];
}

/*
 
 Actions & Callbacks
 
 */

- (void) RequestAccelerometerData:(CDVInvokedUrlCommand*)command
{
    [self StoreCallbackId:command.callbackId forKey:ACCELEROMETER_DATA];
    [self.mza requestDeviceAccelerometerData];
    NSLog(@"RequestAccelerometerData success");
}

- (void) AccelerometerDataReceived:(NSDictionary*)accelData
{
    if ([self GetCallbackId:ACCELEROMETER_DATA])
    {
        NSLog(@"AccelerometerDataReceived success");
        CDVPluginResult* pluginResult = nil;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK
                                     messageAsDictionary:accelData];
        [pluginResult setKeepCallbackAsBool:YES];
        
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:ACCELEROMETER_DATA]];
    }
}

- (void) SetGestureActionKey:(CDVInvokedUrlCommand*)command {
    [self StoreCallbackId:command.callbackId forKey:GESTURE_ACTION_KEY];
    
    NSUInteger gestureKey = [[command.arguments objectAtIndex:0] unsignedLongValue];
    NSString* gestureType = [command.arguments objectAtIndex:1];
    
    if (gestureKey > 0 && gestureType != nil)
    {
        [self.mza setGestureActionKey:gestureKey forGestureType:gestureType];
        NSLog(@"SetGestureActionKey success");
    }
    else
    {
        NSLog(@"SetGestureActionKey failed");
        CDVPluginResult* pluginResult = nil;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR
                                         messageAsString:@"Bad args, no gesture key was assigned."];
        [pluginResult setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:GESTURE_ACTION_KEY]];
    }
}

- (void) RegisterGestureReceived:(CDVInvokedUrlCommand*)command {
    [self StoreCallbackId:command.callbackId forKey:GESTURE_RECEIVED];
}

- (void) GestureReceived:(NSInteger)gestureIndex
{
    if ([self GetCallbackId:GESTURE_RECEIVED])
    {
        NSLog(@"GestureReceived success");
        CDVPluginResult* pluginResult = nil;
        
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDouble:gestureIndex];
        [pluginResult setKeepCallbackAsBool:YES];
        
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:GESTURE_RECEIVED]];
    }
}

- (void) CheckIsAuthorized:(CDVInvokedUrlCommand *)command
{
    NSLog(@"CheckIsAuthorized success");
    BOOL isAuthorized = [self.mza checkIsAuthorized];
    
    CDVPluginResult* pluginResult = nil;
    
    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsBool:isAuthorized];
    [pluginResult setKeepCallbackAsBool:YES];
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:command.callbackId];
    
}

- (void) RequestIsDevAuthorized:(CDVInvokedUrlCommand *)command
{
    [self StoreCallbackId:command.callbackId forKey:DEV_AUTHORIZED];
    [self.mza requestIsDevAuthorized];
    NSLog(@"RequestIsDevAuthorized success");
}

- (void)receivedAuthorization:(BOOL)isAuthorized withError:(NSString*)error
{
    NSLog(@"ReceivedAuthorization success");
    
    NSDictionary* response = @{@"Authorization" : [NSNumber numberWithBool:isAuthorized], @"Error": error};
    
    CDVPluginResult* pluginResult = nil;
    
    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:response];
    [pluginResult setKeepCallbackAsBool:YES];
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:[self GetCallbackId:DEV_AUTHORIZED]];
}

- (void) SetAccelerometerDataReceiveBuffer:(CDVInvokedUrlCommand *)command
{
    BOOL on = [[command.arguments objectAtIndex:0] boolValue];
    
    [self.mza setAccelerometerDataReceiveBuffer:on];
    
    NSLog(@"SetAccelerometerDataReceiveBuffer success");
}

- (void) WasAttached:(BOOL)attached
{
}
- (void) WasConnected:(BOOL)connected
{
}

- (void) RequestBatteryLevelPercent:(CDVInvokedUrlCommand *)command
{
    [self StoreCallbackId:command.callbackId forKey:BATTERY_LEVEL];
    [self.mza requestBatteryLevelPercent];
    NSLog(@"RequestBatteryLevelPercent success");
}

- (void) BatteryLevelReceived:(NSInteger)batteryPercentage
{
    if ([self GetCallbackId:BATTERY_LEVEL])
    {
        NSLog(@"BatteryLevelReceived success");
        CDVPluginResult* pluginResult = nil;
        
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDouble:batteryPercentage];
        [pluginResult setKeepCallbackAsBool:YES];
        
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:BATTERY_LEVEL]];
    }
}

- (void) AutoPlaySettingReceived:(BOOL)autoPlay
{
}
- (void) ChargeStatusReceived:(BOOL)charging
{
}
- (void) BTLocalNameReceived:(NSString*)localName
{
}
- (void) MspVersionReceived:(NSString*)mspVersion
{
}

- (void) RequestDeviceMetadata:(CDVInvokedUrlCommand*)command
{
    [self StoreCallbackId:command.callbackId forKey:DEVICE_METADATA];
    [self.mza requestDeviceMetaData];
    NSLog(@"RequestDeviceMetadata success");
}

- (void) DeviceMetaDataReceived:(NSDictionary*)metaData
{
    if ([self GetCallbackId:DEVICE_METADATA])
    {
        NSLog(@"DeviceMetaDataReceived success");
        CDVPluginResult* pluginResult = nil;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK
                                     messageAsDictionary:metaData];
        [pluginResult setKeepCallbackAsBool:YES];
        
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:DEVICE_METADATA]];
    }
}

- (void) RequestTrackInfo:(CDVInvokedUrlCommand *)command
{
    [self StoreCallbackId:command.callbackId forKey:TRACK_INFO];
    [self.mza requestTrackInfo];
    NSLog(@"RequestTrackInfo success");
}

- (void) TrackInfoReceived:(NSDictionary*)trackInfo
{
    if ([self GetCallbackId:TRACK_INFO])
    {
        NSLog(@"TrackInfoReceived success");
        CDVPluginResult* pluginResult = nil;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK
                                     messageAsDictionary:trackInfo];
        [pluginResult setKeepCallbackAsBool:YES];
        
        [self.commandDelegate sendPluginResult:pluginResult
                                    callbackId:[self GetCallbackId:TRACK_INFO]];
    }
}

@end
