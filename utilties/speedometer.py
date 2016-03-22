import math

def create_line(angle, length, radius, x, y):
	x0 = round(x - radius * math.cos(math.radians(angle)))
	y0 = round(y - radius * math.sin(math.radians(angle)))
	x1 = round(x - (radius - length) * math.cos(math.radians(angle)))
	y1 = round(y - (radius - length) * math.sin(math.radians(angle)))
	return str(int(x0)) + ',' + str(int(y0)) + ',' + str(int(x1)) + ',' + str(int(y1)) + ','

if __name__ == '__main__':
	length = 42
	radius = 250
	x = 507
	y = 318
	print('const int dial_lines[] = {')
	for angle in range(0, 91, 15):
		print('\t' + create_line(angle, length, radius, x, y))
	print("};")

	length = 22
	radius = 245
	print('const int speed_lines[] = {')
	for line in range(1, 66):
		if(line%11 != 0):
			angle = 90.0/66.0 * line
			print('\t' + create_line(angle, length, radius, x, y))
	print("};")
