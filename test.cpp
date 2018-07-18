//
//  test.cpp
//  OverAnimate
//
//  Created by Nevyn Bengtsson on 2016-08-25.
//  Copyright © 2016 ThirdCog. All rights reserved.
//

#include <stdio.h>
#include "OverAnimate.h"

void doIt(int i, float time) {
    printf("lol %d %f\n", i, time);
}

int main(int argc, char **argv) {
    AnimationSystem system;
    BoundFunctionAnimation anim(doIt, 0);
    anim.beginTime = 5;
    anim.duration = 2;
    system.addAnimation(&anim);
    while(true) {
        system.playElapsedTime(0.1);
        if(!anim.scheduled) {
            return 0;
        }
    }
    return 0;
}
