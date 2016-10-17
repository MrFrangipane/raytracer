function format_matrix node_ = (
	t = (matrix3 [1,0,0] [0,0,1] [0,-1,0] [0,0,0]) * node_.transform * inverse(matrix3 [1,0,0] [0,0,1] [0,-1,0] [0,0,0])
	t = node_.transform
	
	return (t[1][1] as string + ", " + t[1][2] as string + ", " + t[1][3] as string + ", 0,\n" +
			t[2][1] as string + ", " + t[2][2] as string + ", " + t[2][3] as string + ", 0,\n" +
			t[3][1] as string + ", " + t[3][2] as string + ", " + t[3][3] as string + ", 0,\n" +
			t[4][1] as string + ", " + t[4][2] as string + ", " + t[4][3] as string + ", 1"
	)
)