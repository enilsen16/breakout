//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "spl/gevents.h"
#include "spl/gobjects.h"
#include "spl/gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 8

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
    double YVelocity = drand48()+2.0;
    double XVelocity = 2.0;
    while (lives > 0 && bricks > 0)
    {
      GEvent event = getNextEvent(MOUSE_EVENT);
      if (event != NULL)
        {
          if (getEventType(event) == MOUSE_MOVED)
            {
              double x = getX(event) - getWidth(paddle) / 2;
              setLocation(paddle, x, 525);
            }
        }
        move(ball, XVelocity, YVelocity);
        if (getY(ball) + getHeight(ball) >= getHeight(window))
           {
            removeGWindow(window, ball);
            lives -= 1;
            ball = initBall(window);
            removeGWindow(window, paddle);
            paddle = initPaddle(window);
            if (lives != 0) waitForClick();
          }

          else if (getY(ball) <= 0)
          {
            YVelocity = -YVelocity;
          }
          if (getX(ball) + getWidth(ball) >= getWidth(window))
            {
              XVelocity = -XVelocity;
            }

            else if (getX(ball) <= 0)
            {
              XVelocity = -XVelocity;
            }

          pause(5);

          GObject object = detectCollision(window, ball);
          if ( object != NULL)
            {
          if (object == paddle)
            {
              YVelocity = -YVelocity;
            }
        else if (strcmp(getType(object), "G3DRect") == 0)
          {
            removeGWindow(window, object);
            YVelocity = -YVelocity;
            bricks -= 1;
            points += 1;
            updateScoreboard(window, label, points);
          }

            }
    }

    //wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
  string color[5] = {"GREEN", "YELLOW", "BLUE", "RED", "ORANGE"};
  srand ( time(NULL) );
  for (int i = 0; i < 8; i++ )
    {
    for (int n = 0; n < 5; n++ )
      {
  G3DRect brick = newG3DRect((26.4 + (43.9 * i)), 50 + (22 * n), 40, 20, true);
  setColor(brick, color[rand()%5]);
  setFilled(brick, true);
  add(window, brick);

      }
  }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
  GOval ball = newGOval(190, 400, 20, 20);
  setColor(ball, "BLACK");
  setFilled(ball, true);
  add(window, ball);
  return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(175, 525, 50, 10);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;

}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
  double x, y;
  GLabel label = newGLabel("0");
  x = (getWidth(window) - getWidth(label)) / 2;
  y = (getHeight(window) + getFontAscent(label)) / 2;
  setLocation(label, x, y);
  add(window, label);
  return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
