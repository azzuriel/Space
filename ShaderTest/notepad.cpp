#ifdef JARG_SOMETHING_UNDEFINED

////////////////////////////////////////////////////////////////////////// canyon spikes like
float t = simplexnoise(offset_x + i/64.0F, offset_y + j/64.0F) + simplexnoise(offset_x + i/32.0F, offset_y + j/32.0F)/2.0F + simplexnoise(offset_x + i/16.0F, offset_y + j/16.0F)/4.0F;
if(t < 0) {
    t = -t;
    t /= 30.0F;
}
if( t < 0.1) {
    t+= 0.1;
}
if(t > .6) {
    t /= 10.0F;
    t += .6;
} 

////////////////////////////////////////////////////////////////////////// canyon spikes like 2
float t = simplexnoise(offset_x + i/64.0F, offset_y + j/64.0F) + simplexnoise(offset_x + i/32.0F, offset_y + j/32.0F)/2.0F + simplexnoise(offset_x + i/16.0F, offset_y + j/16.0F)/4.0F;
if(t < 0) {
    t = -t;
    t /= 30.0F;
}
if( t < 0.1) {
    t+= 0.1;
}
if(t > .6) {
    t /= 10.0F;
    t += .6;
} 
t = sqrtf(t);
#endif