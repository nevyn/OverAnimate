//
//  test.cpp
//  OverAnimate
//
//  Created by Nevyn Bengtsson on 2016-08-25.
//  Copyright © 2016 ThirdCog. All rights reserved.
//

#include <stdio.h>
#include "OverAnimate.h"

void doIt(Animation *self, int i, float time) {
    printf("lol %d %f\n", i, time);
}

int main(int argc, char **argv) {
    AnimationSystem system;
    
    BoundFunctionAnimation anim1(doIt, 0);
    anim1.beginTime = 5;
    anim1.duration = 2;
    system.addAnimation(&anim1);
    
    BoundFunctionAnimation anim2(doIt, 1);
    anim2.beginTime = 6;
    anim2.duration = 2;
    system.addAnimation(&anim2);
    
    while(true) {
        system.playElapsedTime(0.1);
        if(!anim2.scheduled) {
            return 0;
        }
    }
    return 0;
}
