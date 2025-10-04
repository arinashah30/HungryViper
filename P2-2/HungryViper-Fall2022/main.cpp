//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

// PROJECT INCLUDES
#include "globals.h"
#include "graphics.h"
#include "hardware.h"
#include "hash_table.h"
#include "map.h"
#include "viper.h"

#include <math.h>
#include <stdio.h>

// Important definitions

/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper;
bool invincible = false;
bool invincible_forever = false;
bool sticky = false;
int waitTime = 100;
bool scoring = true;
bool paused = false;
int seedNum;
Timer randomTimer;
// bool super = false;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char *wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();
void randomLocation();
void init_second_map();
void start_game();

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS
/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main() {
  randomTimer.start();
  // First things first: initialize hardware
  ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

  viper_init(&viper);

  uLCD.background_color(BLUE);
  uLCD.cls();
  // uLCD.locate(0,0);
  uLCD.color(WHITE);
  uLCD.textbackground_color(BLUE);
  uLCD.set_font(FONT_7X8);
  uLCD.text_mode(OPAQUE);
  uLCD.printf("\n Welcome to Arina's Viper Game! :)\n\n Press button 1 to start the game!");
  while (1) {
    GameInputs inputs = read_inputs();
    int action = get_action(inputs);
    if (action == START_BUTTON) {
      randomTimer.stop();
      break;
    }
  }
  seedNum = randomTimer.read_ms();
  srand(seedNum);
  uLCD.background_color(BLACK);
  uLCD.textbackground_color(BLACK);
  uLCD.cls();

  // 0. Initialize the maps
  // TODO: implement maps_init() and init_main_map() function in map.cpp:
  maps_init();

  init_main_map();

  // Initialize game state
  set_active_map(0);
  viper.head_x = viper.head_y = 5;

  // Initial drawing
  draw_game(FULL_DRAW);
  pc.printf("draw game");

  Timer scoringTimer;
  int scoringTimerTime = 0;

  // Main game loop
  while (1) {

    // Timer to measure game update speed
    Timer t;
    t.start();

    // 1. Read inputs
    // TODO: implement read_inputs() function in hardware.cpp:
    GameInputs inputs = read_inputs();

    // 2. Determine action (move, act, menu, etc.)
    // TODO: implement get_action() function:
    if (sticky) {
      int originalAction = get_action(inputs);
      int action = originalAction;
      /*
      if (originalAction == PAUSE_BUTTON) {
          if (!paused) {
              continue;
          }
      }
      */
      if (paused && action != PAUSE_BUTTON) {
        continue;
      }
      int result = update_game(action);

      if (paused) {
        uLCD.background_color(BLUE);
        uLCD.cls();
        // uLCD.locate(0,0);
        uLCD.color(WHITE);
        uLCD.textbackground_color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.text_mode(OPAQUE);
        uLCD.printf("PAUSED");
        continue;
      }
      while (sticky) {
        if (action != originalAction && action != 0) {
          originalAction = action;
        }
        /*
        if (originalAction == PAUSE_BUTTON) {
            if (!paused) {
                continue;
            }
        }
        */
        if (paused && originalAction != PAUSE_BUTTON) {
          continue;
        }
        result = update_game(originalAction);

        if (paused) {
          uLCD.background_color(BLUE);
          uLCD.cls();
          // uLCD.locate(0,0);
          uLCD.color(WHITE);
          uLCD.textbackground_color(BLUE);
          uLCD.set_font(FONT_7X8);
          uLCD.text_mode(OPAQUE);
          uLCD.printf("PAUSED");
          continue;
        }

        uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
        char str[1024];
        _snprintf(str, sizeof(str), "Pos:%d,%d Score:%d", viper.head_x,
                  viper.head_y, viper.score);
        uLCD.text_string(str, 0, 0, FONT_7X8, RED);
        if (result == GAME_OVER) {
          uLCD.background_color(BLUE);
          uLCD.cls();
          // uLCD.locate(0,0);
          uLCD.color(WHITE);
          uLCD.textbackground_color(BLUE);
          uLCD.set_font(FONT_7X8);
          uLCD.text_mode(OPAQUE);
          uLCD.printf("GAME OVER");
          return NULL;
        } else if (result == GOT_WATER) {
          scoringTimer.start();
        } else if (result == WON_GAME) {
          uLCD.background_color(BLUE);
          uLCD.cls();
          // uLCD.locate(0,0);
          uLCD.color(WHITE);
          uLCD.textbackground_color(BLUE);
          uLCD.set_font(FONT_7X8);
          uLCD.text_mode(OPAQUE);
          uLCD.printf("YOU WIN!");
          break;
        }
        draw_game(result);
        inputs = read_inputs();
        action = get_action(inputs);
        pc.printf("%d", action);
        pc.printf("%d", originalAction);
        pc.printf("%s", sticky ? "true" : "false");
        scoringTimerTime = scoringTimer.read_ms();
        if (scoringTimerTime >= 5000) {
          scoring = true;
          scoringTimer.stop();
        }
      }
    } else {
      int action = get_action(inputs);
      if (paused && action != PAUSE_BUTTON) {
        continue;
      }
      if (action == START_BUTTON) {
            clearMap();
            start_game();
            inputs = read_inputs();
            action = get_action(inputs);
      }

      // 3. Update game
      // TODO: implement update_game() function:
      /*
      if (action == PAUSE_BUTTON) {
          if (!paused) {
              continue;
          }
      }
      */
      int result = update_game(action);

      if (paused) {
        uLCD.background_color(BLUE);
        uLCD.cls();
        uLCD.locate(0, 0);
        uLCD.color(WHITE);
        uLCD.textbackground_color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.text_mode(OPAQUE);
        uLCD.printf("PAUSED");
        continue;
      }

      uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
      char str[1024];
      _snprintf(str, sizeof(str), "Pos:%d,%d Score:%d", viper.head_x,
                viper.head_y, viper.score);
      uLCD.text_string(str, 0, 0, FONT_7X8, RED);

      // 3b. Check for game over based on result
      // and if so, handle game over

      // TODO: implement this here or as a new function.

      //      3b1. if game is over, then
      //      3b2. draw and display text for game over sign
      if (result == GAME_OVER) {
        uLCD.background_color(BLUE);
        uLCD.cls();
        // uLCD.locate(0,0);
        uLCD.color(WHITE);
        uLCD.textbackground_color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.text_mode(OPAQUE);
        uLCD.printf("GAME OVER");
        break;
      } else if (result == GOT_WATER) {
        pc.printf("started scoring timer");
        scoringTimerTime = 0;
        scoringTimer.reset();
        scoringTimer.start();
      } else if (result == WON_GAME) {
        uLCD.background_color(BLUE);
        uLCD.cls();
        // uLCD.locate(0,0);
        uLCD.color(WHITE);
        uLCD.textbackground_color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.text_mode(OPAQUE);
        uLCD.printf("YOU WIN!");
        break;
      }
      // 4. Draw screen -- provided.
      draw_game(result);
    }

    // Compute update time
    t.stop();
    int dt = t.read_ms();

    scoringTimerTime = scoringTimer.read_ms();
    // pc.printf("%d\n", scoringTimerTime);
    if (scoringTimerTime >= 5000) {
      scoring = true;
      scoringTimer.reset();
      scoringTimerTime = 0;
    }
    if (viper.score >= 15) {
      break;
    }

    // Display and wait
    // NOTE: Text is 8 pixels tall
    if (dt < waitTime)
      wait_ms(waitTime - dt);
  }
}

/**
 * Plays a .wav file
 */
void playSound(char *wav) {
  // OPTIONAL: Implement
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNI_BUTTON 7
#define STICKY_JOYSTICK 8
int get_action(GameInputs inputs) {
  // TODO: Implement

  // 1. Check your action and menu button inputs and return the corresponding
  // action value

  // 2. Check for your navigation switch inputs and return the corresponding
  // action value

  if (!inputs.b1)
    return START_BUTTON;
  else if (!inputs.b2)
    return PAUSE_BUTTON;
  else if (!inputs.b3)
    return OMNI_BUTTON;
  else if (inputs.ns_up)
    return GO_UP;
  else if (inputs.ns_down)
    return GO_DOWN;
  else if (inputs.ns_left)
    return GO_LEFT;
  else if (inputs.ns_right)
    return GO_RIGHT;
  else if (inputs.ns_center)
    return STICKY_JOYSTICK;
  else
    return NO_ACTION;
}

/**
 * This function is called by update game to check when the snake
 * gets an object (in other words, when a snake eats an object). We
 * update the snake and game conditions as needed based on the object type
 */
int get_object() {

  // TODO: Implement

  // 1. Get item at current head location
  MapItem *item = get_current(viper.head_x, viper.head_y);
  int item_type = item->type;
  add_viper_head(viper.head_x, viper.head_y);
  // map_erase(viper.locations[viper.length - 1].x, viper.locations[viper.length
  // - 1].y); map_erase(viper.locations[viper.length - 2].x,
  // viper.locations[viper.length - 2].y);
  // add_viper_tail(viper.locations[viper.length - 2].x,
  // viper.locations[viper.length - 2].y);
  //  2. Initialize a return value variable to denote the item type we got
  int type = 1;
  // 3. Check if item is valid and is a chest
  //    3a. if so, increment the score, extend the length of the viper
  //    3b. Remove the chest
  //    3c. set the return value variable to GOT_LENGTH

  // pc.printf("item type: %d \n", item->type);
  switch (item_type) {
  case CHEST:
    if (scoring) {
      viper.score += 1;
      type = GOT_LENGTH;
    } else {
      type = GOT_OBJ;
    }
    break;
    // 4. Else, check if the item is valid a boost-up/boost-down
    //    4a. if so, set the ability for the viper based on the type of item
    //          for the right amount of time
    //    4b. Remove the boost-up/down item
    //    4c. Set the return value variable to GOT_OBJ
    //    Hint: You have to do this check for all your boost-ups/downs. Might be
    //          easier to use Switch statements.
  case PLANT:
    invincible = true;
    // map_erase(viper.head_x, viper.head_y);
    type = GOT_OBJ;
    break;
  case DOOR:
    // map_erase(viper.head_x, viper.head_y);
    type = GOT_OBJ;
    break;
  case WATER:
    // map_erase(viper.head_x, viper.head_y);
    scoring = false;
    type = GOT_WATER;
    pc.printf("Got water");
    break;
  case KEY:
    // map_erase(viper.head_x, viper.head_y);
    type = GOT_OBJ;
    break;
  case APPLE:
    type = GOT_OBJ;
    waitTime -= 30;
    break;
  case SPIRAL:
    pc.printf("in spiral\n");
    clearMap();
    maps_init();
    //set_active_map(1);
    pc.printf("initialize second map\n");
    init_second_map();
    pc.printf("done initializing now print\n");
    set_active_map(0);
    print_map();
    draw_game(1);
    return 1;
  case TNT:
    pc.printf("Portal\n");
    pc.printf("erasing previous body\n");
    map_erase(viper.head_x, viper.head_y);
    for (int i = 0; i < viper.length; i++) {
      map_erase(viper.locations[i].x, viper.locations[i].y);
    }
    viper.head_x = rand() % 50 + 1;
    viper.head_y = rand() % 50 + 1;
    MapItem *newLocation = get_current(viper.head_x, viper.head_y);
    if (newLocation->type == WALL) {
      randomLocation();
    }
    for (int i = 0; i < viper.length; i++) {
      MapItem *newLoc = get_current(viper.head_x - i, viper.head_y);
      if (newLoc->type == WALL) {
        randomLocation();
      }
    }
    for (int i = 0; i < viper.length; i++) {
      viper.locations[i].x = viper.head_x - i;
      viper.locations[i].y = viper.head_y;
      MapItem *newLocation =
          get_current(viper.locations[i].x, viper.locations[i].y);
    }

    pc.printf("adding new body\n");
    add_viper_head(viper.head_x, viper.head_y);
    for (int i = 1; i <= viper.length - 1; i++) {
      add_viper_body(viper.locations[i].x, viper.locations[i].y);
    }
    add_viper_tail(viper.locations[viper.length - 1].x,
                   viper.locations[viper.length - 1].y);
    return 1;

    // 5. Check if the return value variable is GOT_LENGTH
    //    5a. If so, then increase the snake length and update its locations as
    //    needed
    //        Hint: Refer to Step 6 below; the main difference is that you need
    //        to lengthen the snake (what should we change the snake tail
    //        position to?)
  }
  if (type == GOT_LENGTH) {
    viper.length++; // increase length two times?
    int origX = viper.locations[viper.length - 2].x;
    int origY = viper.locations[viper.length - 2].y;
    for (int i = viper.length - 1; i > 0; i--) {
      viper.locations[i].x = viper.locations[i - 1].x;
      viper.locations[i].y = viper.locations[i - 1].y;
    }
    viper.locations[0].x = viper.head_x;
    viper.locations[0].y = viper.head_y;
    for (int i = 1; i <= viper.length - 1; i++) {
      add_viper_body(viper.locations[i].x, viper.locations[i].y);
    }
    add_viper_tail(viper.locations[viper.length - 1].x,
                   viper.locations[viper.length - 1].y);
  } else {
    int origX = viper.locations[viper.length - 1].x;
    int origY = viper.locations[viper.length - 1].y;
    for (int i = viper.length - 1; i > 0; i--) {
      viper.locations[i].x = viper.locations[i - 1].x;
      viper.locations[i].y = viper.locations[i - 1].y;
    }
    viper.locations[0].x = viper.head_x;
    viper.locations[0].y = viper.head_y;
    for (int i = 1; i <= viper.length - 1; i++) {
      add_viper_body(viper.locations[i].x, viper.locations[i].y);
    }
    add_viper_tail(viper.locations[viper.length - 1].x,
                   viper.locations[viper.length - 1].y);
    map_erase(origX, origY);
  }
  return type;
  // 6. For any other object, do the following default steps
  //    6a. initialize two variables to to denote the original location for the
  //    x and y tail of the viper 6b. For each location of the viper body, move
  //    them forward by one location 6c. Update the initial viper location to
  //    the viper head x and y location 6d. call a function to add a viper head
  //    given the head x and y location 6e. call a function to add a viper body
  //    to the location after the head 6f. call a function to add a viper tail
  //    to the location of the last viper locations 6g. call a function to map
  //    erase the original location for the x and y tail 6h. return the return
  //    value variable
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the viper position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the viper has not moved.
 */
int update_game(int action) {
  // TODO: Implement

  // 1. Check the viper speed and update viper speed status

  // 2. Update the previous viper head position to the current position
  viper.head_px = viper.head_x;
  viper.head_py = viper.head_y;
  // 3. Check if action is to go up
  int obj;
  if (action == GO_UP) {
    MapItem *item = get_north(viper.head_x, viper.head_y);
    viper.head_y -= 1;
    // pc.printf("UP\n");
    // pc.printf("%s", (item == NULL) ? "true" : "false");
    if (item) {
      if (!item->walkable) {
        if (!invincible && !invincible_forever) {
          return GAME_OVER;
        } else {
          invincible = false;
          viper.head_y += 1;
          // make it not be able to go
        }
      }
    }
    if (item->walkable) {
      obj = get_object();
    }
    if (viper.length > VIPER_MAX_LENGTH) {
      return GAME_OVER;
    }
    // print_map();
    if (obj == GOT_WATER) {
      return GOT_WATER;
    } else if (viper.score >= 20) {
      return WON_GAME;
    }
    return MOVED;
  } else if (action == GO_DOWN) {
    MapItem *item = get_south(viper.head_x, viper.head_y);
    viper.head_y += 1;
    // pc.printf("DOWN!\n");
    // pc.printf("%s", (item == NULL) ? "true" : "false");
    if (item) {
      // pc.printf("DOWN!\n");
      //  if (item->walkable) {
      //      viper.head_y -= 1;
      //  } else {
      //      return GAME_OVER;
      //  }
      if (!item->walkable) {
        if (!invincible && !invincible_forever) {
          return GAME_OVER;
        } else {
          invincible = false;
          viper.head_y -= 1;
        }
      }
    }
    if (item->walkable) {
      obj = get_object();
    }
    if (viper.length > VIPER_MAX_LENGTH) {
      return GAME_OVER;
    }
    // print_map();
    if (obj == GOT_WATER) {
      return GOT_WATER;
    } else if (viper.score >= 20) {
      return WON_GAME;
    }
    return MOVED;
  } else if (action == GO_LEFT) {
    MapItem *item = get_west(viper.head_x, viper.head_y);
    viper.head_x -= 1;
    // pc.printf("LEFT!\n");
    // pc.printf("%s", (item == NULL) ? "true" : "false");
    if (item) {
      // pc.printf("LEFT!\n");
      if (!item->walkable) {
        if (!invincible && !invincible_forever) {
          return GAME_OVER;
        } else {
          invincible = false;
          viper.head_x += 1;
        }
      }
    }
    if (item->walkable) {
      obj = get_object();
    }
    if (viper.length > VIPER_MAX_LENGTH) {
      return GAME_OVER;
    }
    // print_map();
    if (obj == GOT_WATER) {
      return GOT_WATER;
    } else if (viper.score >= 20) {
      return WON_GAME;
    }
    return MOVED;
  } else if (action == GO_RIGHT) {
    MapItem *item = get_east(viper.head_x, viper.head_y);
    viper.head_x += 1;
    // pc.printf("RIGHT!\n");
    // pc.printf("%s", (item == NULL) ? "true" : "false");
    if (item) {
      // pc.printf("RIGHT!\n");
      //  if (item->walkable) {
      //      viper.head_x -= 1;
      //  } else {
      //      return GAME_OVER;
      //  }
      if (!item->walkable) {
        if (!invincible && !invincible_forever) {
          return GAME_OVER;
        } else {
          invincible = false;
          viper.head_x -= 1;
        }
      }
    }
    // add_viper_head(viper.head_x, viper.head_y);
    if (item->walkable) {
      obj = get_object();
    }
    if (viper.length > VIPER_MAX_LENGTH) {
      return GAME_OVER;
    }
    // print_map();
    if (obj == GOT_WATER) {
      return GOT_WATER;
    } else if (viper.score >= 20) {
      return WON_GAME;
    }
    return MOVED;
  } else if (action == OMNI_BUTTON) {
    if (invincible_forever) {
      invincible_forever = false;
      pc.printf("invincible forever off");
    } else {
      invincible_forever = true;
      pc.printf("invincible forever on");
    }

  } else if (action == STICKY_JOYSTICK) {
    if (sticky) {
      sticky = false;
      pc.printf("sticky off");
    } else {
      sticky = true;
      pc.printf("sticky on");
    }
  } else if (action == PAUSE_BUTTON) {
    if (paused) {
      paused = false;
      pc.printf("paused off");
      uLCD.background_color(BLACK);
      uLCD.textbackground_color(BLACK);
      uLCD.cls();
      draw_border();
      draw_game(1);
    } else {
      paused = true;
      pc.printf("paused on");
    }
  }
  //      3a. Get item north of the current viper head
  //      3b. Update the current viper head position upwards
  //      3c. Check if the item exist and item is walkable
  //          3c1. Check if the viper head does not collide with itself
  //          3c2. If so, return game over
  //          3c3. Otherwise, update viper head back to its original position
  //               and return no result.
  //      3d. Check if the viper gets an object (eats) by calling another
  //      function
  //          3d1. Check if viper length is more than the maximum length
  //          3d2. If so, return game over
  //  ````3e. Finally, return moved since the viper has moved

  // 4. Else check if action is to go left, right, and down
  //    NOTE: Repeat same general steps as in go up

  // If no action was given, then we simply return no result

  return NO_RESULT;
}

/**
 * Draw the upper status bar.
 */
void draw_upper_status() { uLCD.line(0, 9, 127, 9, GREEN); }

/**
 * Draw the lower status bar.
 */
void draw_lower_status() { uLCD.line(0, 118, 127, 118, GREEN); }

/**
 * Draw the border for the map.
 */
void draw_border() {
  uLCD.filled_rectangle(0, 9, 127, 14, WHITE);     // Top
  uLCD.filled_rectangle(0, 13, 2, 114, WHITE);     // Left
  uLCD.filled_rectangle(0, 114, 127, 117, WHITE);  // Bottom
  uLCD.filled_rectangle(124, 14, 127, 117, WHITE); // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option) {
  // Draw game border first
  if (draw_option == FULL_DRAW) {
    draw_border();
    int u = 58;
    int v = 56;
    draw_viper_head(u, v);
    draw_viper_body(u - 11, v);
    draw_viper_tail(u - 22, v);
    return;
  }

  // Iterate over all visible map tiles
  for (int i = -5; i <= 5; i++) {   // Iterate over columns of tiles
    for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
      // Here, we have a given (i,j)

      // Compute the current map (x,y) of this tile
      int x = i + viper.head_x;
      int y = j + viper.head_y;

      // Compute the previous map (px, py) of this tile
      int px = i + viper.head_px;
      int py = j + viper.head_py;

      // Compute u,v coordinates for drawing
      int u = (i + 5) * 11 + 3;
      int v = (j + 4) * 11 + 15;

      // Figure out what to draw
      DrawFunc draw = NULL;
      if (x >= 0 && y >= 0 && x < map_width() &&
          y < map_height()) { // Current (i,j) in the map
        MapItem *curr_item = get_here(x, y);
        MapItem *prev_item = get_here(px, py);
        if (draw_option ||
            curr_item != prev_item) { // Only draw if they're different
          if (curr_item) {            // There's something here! Draw it
            draw = curr_item->draw;
          } else { // There used to be something, but now there isn't
            draw = draw_nothing;
          }
        } else if (curr_item && curr_item->type == CLEAR) {
          // This is a special case for erasing things like doors.
          draw = curr_item->draw; // i.e. draw_nothing
        }
      } else if (draw_option) { // If doing a full draw, but we're out of
                                // bounds, draw the walls.
        draw = draw_wall;
      }

      // Actually draw the tile
      if (draw)
        draw(u, v);
    }
  }

  // Draw status bars
  draw_upper_status();
  draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map() {
  // "Random" plants
  Map *map = set_active_map(0);
  /*
  for(int i = map_width() + 3; i < map_area(); i += 39) {
      add_chest(i % map_width(), i / map_width());
  }
  */
  for (int i = 0; i < 40; i++) {
    int xcoord = rand() % 50 + 1;
    int ycoord = rand() % 50 + 1;
    add_chest(xcoord, ycoord);
  }
  pc.printf("plants\r\n");
  for (int i = 0; i < 20; i++) {
    int xcoord = rand() % 50 + 1;
    int ycoord = rand() % 50 + 1;
    add_plant(xcoord, ycoord);
  }


  for (int i = 0; i < 10; i++) {
    int xcoord = rand() % 50 + 1;
    int ycoord = rand() % 50 + 1;
    add_TNT(xcoord, ycoord);
  }

  for (int i = 0; i < 20; i++) {
    int xcoord = rand() % 50 + 1;
    int ycoord = rand() % 50 + 1;
    add_water(xcoord, ycoord);
  }

  for (int i = 0; i < 15; i++) {
    int xcoord = rand() % 50 + 1;
    int ycoord = rand() % 50 + 1;
    add_apple(xcoord, ycoord);
  }

  add_spiral(rand() % 50 + 1, rand() % 50 + 1);

  pc.printf("Adding walls!\r\n");
  add_wall(0, 0, HORIZONTAL, map_width());
  add_wall(0, map_height() - 1, HORIZONTAL, map_width());
  add_wall(0, 0, VERTICAL, map_height());
  add_wall(map_width() - 1, 0, VERTICAL, map_height());
  pc.printf("Walls done!\r\n");

  add_viper_head(viper.locations[0].x, viper.locations[0].y);
  add_viper_body(viper.locations[1].x, viper.locations[1].y);
  add_viper_tail(viper.locations[2].x, viper.locations[2].y);

  pc.printf("Add extra chamber\r\n");
  add_wall(30, 0, VERTICAL, 10);
  add_wall(30, 10, HORIZONTAL, 10);
  add_wall(39, 0, VERTICAL, 10);
  pc.printf("Added!\r\n");

  print_map();
}

void randomLocation() {
  map_erase(viper.head_x, viper.head_y);
  for (int i = 0; i < viper.length; i++) {
    map_erase(viper.locations[i].x, viper.locations[i].y);
  }
  viper.head_x = rand() % 50 + 1;
  viper.head_y = rand() % 50 + 1;
}

void init_second_map() {
    

    pc.printf("new screen");
    uLCD.background_color(BLUE);
    uLCD.cls();
    uLCD.color(WHITE);
    uLCD.textbackground_color(BLUE);
    uLCD.set_font(FONT_7X8);
    uLCD.text_mode(OPAQUE);
    uLCD.printf("You found the infinite map of chests!");
    wait_ms(3000);
    uLCD.background_color(BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.cls();
    pc.printf("set_active_map\n");
    Map *map = set_active_map(0);
    pc.printf("adding chests\n");
    for (int i = map_width() + 3; i < map_area(); i += 39) {
        add_chest(i % map_width(), i / map_width());
    }
    pc.printf("adding walls\n");
    add_wall(0, 0, HORIZONTAL, map_width());
    add_wall(0, map_height() - 1, HORIZONTAL, map_width());
    add_wall(0, 0, VERTICAL, map_height());
    add_wall(map_width() - 1, 0, VERTICAL, map_height());

  viper.head_x = rand() % 50 + 1;
  viper.head_y = rand() % 50 + 1;
  MapItem *newLocation = get_current(viper.head_x, viper.head_y);
  if (newLocation->type == WALL) {
    randomLocation();
  }
  for (int i = 0; i < viper.length; i++) {
    MapItem *newLoc = get_current(viper.head_x - i, viper.head_y);
    if (newLoc->type == WALL) {
      randomLocation();
    }
  }
  for (int i = 0; i < viper.length; i++) {
    viper.locations[i].x = viper.head_x - i;
    viper.locations[i].y = viper.head_y;
    MapItem *newLocation =
        get_current(viper.locations[i].x, viper.locations[i].y);
  }

  pc.printf("adding new body\n");
  add_viper_head(viper.head_x, viper.head_y);
  for (int i = 1; i <= viper.length - 1; i++) {
    add_viper_body(viper.locations[i].x, viper.locations[i].y);
  }
  add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
  print_map();
}

void start_game() {
    randomTimer.start();

  viper_init(&viper);

  uLCD.background_color(BLUE);
  uLCD.cls();
  // uLCD.locate(0,0);
  uLCD.color(WHITE);
  uLCD.textbackground_color(BLUE);
  uLCD.set_font(FONT_7X8);
  uLCD.text_mode(OPAQUE);
  uLCD.printf("\n RESTARTING...");
  wait_ms(2000);
  uLCD.cls();
  uLCD.printf("\n Welcome to Arina's Viper Game! :)\n\n Press button 1 to start the game!");
  while (1) {
    GameInputs inputs = read_inputs();
    int action = get_action(inputs);
    if (action == START_BUTTON) {
      randomTimer.stop();
      break;
    }
  }
  seedNum = randomTimer.read_ms();
  srand(seedNum);
  uLCD.background_color(BLACK);
  uLCD.textbackground_color(BLACK);
  uLCD.cls();

  // 0. Initialize the maps
  // TODO: implement maps_init() and init_main_map() function in map.cpp:
  maps_init();

  init_main_map();

  // Initialize game state
  set_active_map(0);
  viper.head_x = viper.head_y = 5;

  // Initial drawing
  draw_game(FULL_DRAW);
  pc.printf("draw game");

}