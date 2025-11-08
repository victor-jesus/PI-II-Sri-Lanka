#include "box.h"
#include <stdbool.h>

bool check_collision_between_hit_boxes(Box a, Box b){
    return !(a.x + a.w < b.x ||  
        a.x > b.x + b.w ||  
        a.y + a.h < b.y ||  
        a.y > b.y + b.h);   
}