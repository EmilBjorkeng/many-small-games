import pygame
import random

height = 20
width = 20
grid_size = 20
fps = 100
movement_update_time = 200

running = True
move_update = movement_update_time
snake_pos = [[int(width / 2), int(height / 2)]]
apple_pos = [int(width / 2) + 3, int(height / 2)]
vel = [1, 0]
input_stack = []

# Game Over
def game_over(reason):
	global running
	print("Game Over")
	print("Reason: " + reason)
	print("Length: " + str(len(snake_pos)))
	running = False

# Pygame init
pygame.init()
display = pygame.display.set_mode((grid_size * width, grid_size * height))
pygame.display.set_caption('Snake')
clock = pygame.time.Clock()

# Game Loop
while running:

	# Delta Time
	dt = clock.tick(fps)

	# Events
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			running = False

		# Keyboard Input
		if event.type == pygame.KEYDOWN:
			if event.key == pygame.K_UP:
				input_stack.append([0, -1])

			if event.key == pygame.K_DOWN:
				input_stack.append([0, 1])

			if event.key == pygame.K_LEFT:
				input_stack.append([-1, 0])

			if event.key == pygame.K_RIGHT:
				input_stack.append([1, 0])

			# debug Key
			if event.key == pygame.K_g:
				snake_pos.append([-1, -1])

	# Movement Update
	if move_update <= 0:
		move_update = movement_update_time

		# Input stack
		if len(input_stack):
			
			# Loop in case there is impossible moves
			while True:
				if not len(input_stack):
					break

				# Get velosity from the input stack
				if not input_stack[0][0] and vel[0] or not input_stack[0][1] and vel[1]:
					vel[0] = input_stack[0][0]
					vel[1] = input_stack[0][1]
					del input_stack[0]
					break
			
				del input_stack[0]

		# Calculate Snake Tail
		for i in range(len(snake_pos) - 1, 0, -1):
			snake_pos[i][0] = snake_pos[i - 1][0]
			snake_pos[i][1] = snake_pos[i - 1][1]

		# Move Snake Head
		snake_pos[0][0] = snake_pos[0][0] + vel[0]
		snake_pos[0][1] = snake_pos[0][1] + vel[1]
	else:
		move_update -= dt

	# Hit Tail
	for i in range(1, len(snake_pos), 1):
		if snake_pos[0][0] == snake_pos[i][0] and snake_pos[0][1] == snake_pos[i][1]:
			game_over("Hit Tail")

	# Hit Wall
	if snake_pos[0][0] < 0 or snake_pos[0][0] > width - 1 or snake_pos[0][1] < 0 or snake_pos[0][1] > height - 1:
		game_over("Hit Wall")

	# Eat Apple
	if snake_pos[0][0] == apple_pos[0] and snake_pos[0][1] == apple_pos[1]:
		snake_pos.append([-1, -1])
		
		# Random Apple Posision
		while True:
			x = random.randrange(0, width, 1)
			y = random.randrange(0, height, 1)
			something_there = False

			for i in range(0, len(snake_pos), 1):
				if snake_pos[i][0] == x and snake_pos[i][1] == y:
					something_there = True

			if not something_there:
				apple_pos = [x, y]
				break

#	# Screen Wrapping
#	if snake_pos[0][0] < 0:
#		snake_pos[0][0] = width - 1
#
#	if snake_pos[0][0] > width - 1:
#		snake_pos[0][0] = 0
#
#	if snake_pos[0][1] < 0:
#		snake_pos[0][1] = height - 1
#
#	if snake_pos[0][1] > height - 1:
#		snake_pos[0][1] = 0


	# Draw
	display.fill((255, 255, 255))
	
	# Apple
	pygame.draw.rect(display, (255, 0, 0), (apple_pos[0] * grid_size, apple_pos[1] * grid_size, grid_size, grid_size))

	# Snake
	for i in range(1, len(snake_pos), 1):
		pygame.draw.rect(display, (25, 100 + (i % 2 * 100), 25), (snake_pos[i][0] * grid_size, snake_pos[i][1] * grid_size, grid_size, grid_size))

	pygame.draw.rect(display, (250, 240, 50), (snake_pos[0][0] * grid_size, snake_pos[0][1] * grid_size, grid_size, grid_size))

	# Update Screen
	pygame.display.update()

# Quit the game
pygame.quit()
quit()

