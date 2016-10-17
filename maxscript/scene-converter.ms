struct FrangiRayConverter
(
	function format_matrix t =
	(
		-- Return
		(	
			t[1][1] as string + " " + t[1][2] as string + " " + t[1][3] as string + " 0 " +
			t[2][1] as string + " " + t[2][2] as string + " " + t[2][3] as string + " 0 " +
			t[3][1] as string + " " + t[3][2] as string + " " + t[3][3] as string + " 0 " +
			t[4][1] as string + " " + t[4][2] as string + " " + t[4][3] as string + " 1"
		)
	),

	fn convert_viewport =
	(
		-- Get Viewport infos
		viewport_fov = getViewFOV() as string
		viewport_matrix = this.format_matrix (getViewTM())
		-- Return line
		(
			"camera " + viewport_fov + " " + viewport_matrix
		)
	),
	
	fn convert_sphere sphere_ =
	(
		-- Get infos
		radius = sphere_.radius as string
		pos_x = sphere_.position.x as string
		pos_y = sphere_.position.y as string
		pos_z = sphere_.position.z as string
		emission_r = 0 as string
		emission_g = 0 as string
		emission_b = 0 as string
		-- Has Standard Material
		if classof sphere_.material != Standardmaterial then
		(
			diffuse_r = (sphere_.wirecolor.r / 255) as string
			diffuse_g = (sphere_.wirecolor.g / 255) as string
			diffuse_b = (sphere_.wirecolor.b / 255) as string
		)
		-- Use Wirecolor
		else
		(
			diffuse_r = (sphere_.material.diffuse.r / 255) as string
			diffuse_g = (sphere_.material.diffuse.g / 255) as string
			diffuse_b = (sphere_.material.diffuse.b / 255) as string
			if sphere_.material.useSelfIllumColor then
			(
				emission_r = (sphere_.material.selfIllumColor.r / 255) as string
				emission_g = (sphere_.material.selfIllumColor.g / 255) as string
				emission_b = (sphere_.material.selfIllumColor.b / 255) as string
			)
		)
		-- Return line
		(
			"sphere " + radius + " " + pos_x + " " + pos_y + " " + pos_z + " " + diffuse_r + " " + diffuse_g + " " + diffuse_b + " " + emission_r + " " + emission_g + " " + emission_b
		)	
	),
	
	fn convert =
	(
		-- Return
		return_ = ""
		-- Viewport
		return_ = return_ + "\nFrangiRay Scene Description\n"
		return_ = return_ + ((this.convert_viewport()) + "\n")
		-- Spheres
		spheres = for node_ in geometry where classof node_ == sphere collect node_
		
		for sphere_ in spheres do
		(
			return_ = return_ + ((this.convert_sphere sphere_)  + "\n")
		)
		-- End
		return_ = return_ + "\n"
		-- Return
		return_
	),
	
	fn save_to filepath =
	(
		file = createFile filepath
		format (this.convert()) to:file
		close file
	)
)


FrangiRayConverter = FrangiRayConverter()

macroScript FrangiRay_SaveTo
	category:"#FrangiRay"
	toolTip:"FrangiRay convert scene"
(
	FrangiRayConverter.save_to @"D:\dev\raytracer\raytracer\scene.txt"
)

