// ============================================
// The Viper class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "viper.h"
#include <cstddef>

void viper_init (Viper * v)
{
    // TODO: Implement
    //1. Set starting coordinates for your viper head and previous
    v->head_x = 5;
    v->head_y = 5;
    v->head_px = 4;
    v->head_py = 5;
    //2. Initialize all location for your maximum viper body (loop through the viper)
    for (int i = 0; i < v->length; i++) {
        v->locations[i].x = v->head_x - i;
        v->locations[i].y = v->head_y;
    }
    //3. Set starting length of your viper
    v->length = 3;
    //4. Set starting coordinates for the rest of viper body from starting length
    v->locations[0].x = v->head_x;
    v->locations[0].y = v->head_y;
    v->locations[1].x = v->head_x - 1;
    v->locations[1].y = v->head_y;
    v->locations[2].x = v->head_x - 2;
    v->locations[2].y = v->head_x;
    //5. Initialize viper status
    v->score = 0;
    
    
    

}
