//
//  AddressBookElement.h
//  CGModule
//
//  Created by Cornelli Fabio on 30/10/17.
//  Copyright © 2017 Cornelli Fabio. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AddressBookElement : NSObject

@property(nonatomic) NSString* elementId;
@property(nonatomic) NSString* name;

-(AddressBookElement*) initWithId:(NSString*)elementId andName:(NSString*)name;

@end
