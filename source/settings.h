#ifndef SETTINGS_H
#define SETTINGS_H

// this is fine, but it won't get saved in the executable if you change it during execution,
// if you want to have it save you might need to use a non-code file there's also probably
// a way to actually save it in the executable, if you want to look for it 

// these get hidden by local variables called width/height
const int gameWidth = 10;
const int gameHeight = 20;
double timeForDrop;
double timeForDrop2;
const double updateTime = (double)1 / (double)60;

double moveTime = 0.15;
double rotateTime = 0.3;
double softDropMult = 0.5;

#endif