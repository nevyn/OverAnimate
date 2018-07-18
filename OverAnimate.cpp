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
    for(int i = 0; i < _lowestAvailableIndex; i++) {
        if(_animations[i] == animation) {
            animation->scheduled = true;
            break;
        }
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
        if(_elapsedTime > animation->beginTime) {
            if(_elapsedTime - animation->beginTime > animation->duration) {
                removeAnimation(animation);
                i--; // mmmm, mutating while iterating...
                continue;
            }
            animation->animate(_elapsedTime);
        }
    }
}

