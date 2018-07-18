#include "OverAnimate.h"

Animation::
Animation() :
    beginTime(0),
    duration(0)
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
    this->function(boundArgument, absoluteTime);
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
    if(_lowestAvailableIndex == kMaxAnimationCount-1) {
        // full, don't add any more
        return;
    }

    _animations[_lowestAvailableIndex] = animation;
    animation->scheduled = true;
    _lowestAvailableIndex = _findLowestIndexAbove(_lowestAvailableIndex);
}

void
AnimationSystem::
playElapsedTime(
    float deltaAdded
) {
    _elapsedTime += deltaAdded;
    
    for(int i = 0; i < kMaxAnimationCount; i++) {
        Animation *animation = _animations[i];
        if(!animation) {
            continue;
        }
        if(_elapsedTime > animation->beginTime) {
            if(_elapsedTime - animation->beginTime > animation->duration) {
                animation->scheduled = false;
                _animations[i] = NULL;
                continue;
            }
            animation->animate(_elapsedTime);
        }
    }
}

int
AnimationSystem::
_findLowestIndexAbove(
    int minIndex
) {
    for(int i = minIndex; i < kMaxAnimationCount; i++) {
        if(_animations[i] == NULL) {
            return i;
        }
    }
    return kMaxAnimationCount;
}
