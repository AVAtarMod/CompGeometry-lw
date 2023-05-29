import pygame
import numpy
from math import pi, sin, cos

# some constants
FPS = 10
WINDOW_SIZE = [400, 400]
LINE_WIDTH = 1
MIN_LINE_LENGTH = 1  # when to terminate recursion

# main loop


def main():
    # init pygame and window
    pygame.init()
    screen = pygame.display.set_mode(WINDOW_SIZE)
    # loop until user exits
    done = False
    clock = pygame.time.Clock()
    while not done:
        # limit frame rate
        clock.tick(FPS)
        # check if user exited
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
        # clear screen
        screen.fill([255, 255, 255])
        # calculate the three corners of the snowflake,
        # an equilateral triangle centered on the screen
        screen_center = numpy.array([WINDOW_SIZE[0]/2, WINDOW_SIZE[1]/2])
        # not really radius? but distance from center to corner
        radius = WINDOW_SIZE[1]/2
        snowflake_top = screen_center + vector_from_polar(radius, pi/2)
        snowflake_left = screen_center + \
            vector_from_polar(radius, pi/2 + 2*pi/3)
        snowflake_right = screen_center + \
            vector_from_polar(radius, pi/2 + 4*pi/3)
        # draw the snowflake
        draw_koch_line(screen, snowflake_top,   snowflake_left)
        draw_koch_line(screen, snowflake_left,  snowflake_right)
        draw_koch_line(screen, snowflake_right, snowflake_top)
        # flip buffers
        pygame.display.flip()


# vector_from_polar: constructs a vector from its angle and mangitude
def vector_from_polar(magnitude, angle):
    return magnitude * numpy.array([numpy.cos(angle), numpy.sin(angle)])

# draw_koch_line: (approximately) draws a Koch line between the points specified


def draw_koch_line(screen, line_start, line_end):
    if numpy.linalg.norm(line_end - line_start) / 3 < MIN_LINE_LENGTH:
        # last iteration: draw the line
        pygame.draw.line(screen, [0, 0, 0], line_start, line_end, LINE_WIDTH)
    else:
        # find the normal to this line
        line_normal = numpy.array([
            line_end[1]-line_start[1],
            line_start[0]-line_end[0]])
        # find the three points of the "triangle" used to define the segments
        triangle_left = 2/3*line_start + 1/3*line_end  # 1/3 of this line
        triangle_right = 1/3*line_start + 2/3*line_end  # 2/3 of this line
        triangle_top = 1/2*line_start + 1/2*line_end + \
            numpy.sqrt(3)/2/3 * line_normal  # point "above" the line
        # recurse for each segments
        draw_koch_line(screen, line_start,     triangle_left)
        draw_koch_line(screen, triangle_left,  triangle_top)
        draw_koch_line(screen, triangle_top,   triangle_right)
        draw_koch_line(screen, triangle_right, line_end)


main()
