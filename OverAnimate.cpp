#include "OverAnimate.h"

Animation::
Animation() :
    beginTime(0),
    duration(0),
    scheduled(0),
    enabled(1),
    repeats(0)
{

}

void
Animation::
animate(
    float absoluteTime
) {

}

#pragma mark -

BoundFunctionAnimation::
BoundFunctionAnimation(
    AnimationFunction func,
    int boundArgument
) :
    function(func),
    boundArgument(boundArgument)
{

}

void
BoundFunctionAnimation::
animate(
    float absoluteTime
) {
    this->function(this, boundArgument, absoluteTime);
}

#pragma mark -

AnimationSystem::
AnimationSystem() :
    _lowestAvailableIndex(0),
    _elapsedTime(0)
{
    memset(_animations, 0, kMaxAnimationCount*sizeof(Animation*));
}

float
AnimationSystem::
now()  {
    return _elapsedTime;
}

void
AnimationSystem::
addAnimation(
    Animation *animation
) {
    if(animation->scheduled) {
        return;
    }
    if(_lowestAvailableIndex == kMaxAnimationCount-1) {
        // full, don't add any more
        return;
    }

    _animations[_lowestAvailableIndex] = animation;
    animation->scheduled = true;
    _lowestAvailableIndex++;
}

void
AnimationSystem::
removeAnimation(
    Animation *toRemove
) {
    if(!toRemove->scheduled) {
        return;
    }
    for(int i = 0; i < _lowestAvailableIndex; i++) {
        Animation *animation = _animations[i];
        if(animation == toRemove) {
            animation->scheduled = false;
            _animations[i] = NULL;
            // move the rest down
            for(int j = i; j < _lowestAvailableIndex-1; j++) {
                _animations[j] = _animations[j+1];
            }
            break;
        }
    }
    _lowestAvailableIndex--;
}

void
AnimationSystem::
playElapsedTime(
    float deltaAdded
) {
    _elapsedTime += deltaAdded;
    
    for(int i = 0; i < _lowestAvailableIndex; i++) {
        Animation *animation = _animations[i];
        if(!animation->enabled) {
            continue;
        }
        if(_elapsedTime > animation->beginTime) {
            float fraction = (_elapsedTime - animation->beginTime) / animation->duration;
            if(fraction >= 1.0) {
                if(animation->repeats) {
                    animation->beginTime = now();
                } else {
                    removeAnimation(animation);
                    i--;
                    continue;
                }
            }
            animation->animate(fraction);
        }
    }
}

