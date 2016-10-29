struct FrangiRayConverter
(
	last_saved_filepath = "",
	
	function format_matrix t =
	(
		-- Return
		(	
			"            \"transform\": [\n" + 
			"                " + t[1][1] as string + ", " + t[1][2] as string + ", " + t[1][3] as string + ", 0.0,\n" +
			"                " + t[2][1] as string + ", " + t[2][2] as string + ", " + t[2][3] as string + ", 0.0,\n" +
			"                " + t[3][1] as string + ", " + t[3][2] as string + ", " + t[3][3] as string + ", 0.0,\n" +
			"                " + t[4][1] as string + ", " + t[4][2] as string + ", " + t[4][3] as string + ", 1.0\n" +
			"            ],\n"
		)
	),

	fn convert_viewport =
	(
		-- Get Viewport infos
		viewport_fov = getViewFOV() as string
		viewport_matrix = this.format_matrix (getViewTM())
		-- Return block
		(
			"        {\n" +
            "            \"class\": \"camera\",\n" +
			"            \"name\": \"3ds Max Viewport\",\n" +
			viewport_matrix +
			"            \"fov\": " + viewport_fov + ",\n" +
			"            \"exposure\": 1.0\n" +
			"        }"
		)
	),
	
	fn convert_sphere sphere_ =
	(
		-- Get infos
		radius = sphere_.radius as string
		matrix = this.format_matrix sphere_.transform
		emission_r = 0.0 as string
		emission_g = 0.0 as string
		emission_b = 0.0 as string
		-- Has Standard Material
		if classof sphere_.material != Standardmaterial then
		(
			diffuse_r = (sphere_.wirecolor.r / 255) as string
			diffuse_g = (sphere_.wirecolor.g / 255) as string
			diffuse_b = (sphere_.wirecolor.b / 255) as string
			reflection_amount = 0.0 as string
			reflection_blur = 0.0 as string
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
			reflection_amount = (sphere_.material.specularLevel / 100) as string
			reflection_blur = ((100 - sphere_.material.glossiness) / 100) as string
		)
		-- Return line
		(
			"        {\n" +
			"            \"class\": \"sphere\",\n" +
			"            \"name\": \"" + sphere_.name + "\",\n" +
			matrix +
			"            \"radius\": " + radius + ",\n" +
			"            \"diffuse_color\": [" + diffuse_r + ", " + diffuse_g + ", " + diffuse_b + "],\n" +
			"            \"emission_color\": [" + emission_r + ", " + emission_g + ", " + emission_b + "],\n" +
			"            \"reflection_amount\": " + reflection_amount + ",\n" +
			"            \"reflection_roughness\": " + reflection_blur + "\n" +
			"        }"
		)	
	),
	
	fn convert_camera camera_ =
	(
		-- Infos
		fov = camera_.fov as string
		matrix = this.format_matrix camera_.transform
		-- Return block
		(
			"        {\n" +
            "            \"class\": \"camera\",\n" +
			"            \"name\": \"" + camera_.name + "\",\n" +
			matrix +
			"            \"fov\": " + fov + ",\n" +
			"            \"exposure\": 1.0\n" +
			"        }"
		)
	),
	
	fn convert =
	(
		-- Return
		return_ = 	"{\n" +
						"    \"nodes\":\n" +
						"    ["

		-- Viewport
		return_ = return_ + ("\n" + this.convert_viewport())
		
		-- Cameras
		cameras_ = for node_ in cameras where superclassof node_ == camera collect node_
		for camera_ in cameras_ do
		(
			return_ = return_ + (",\n" + this.convert_camera camera_)
		)

		-- Spheres
		spheres = for node_ in geometry where classof node_ == sphere collect node_
		
		for sphere_ in spheres do
		(
			return_ = return_ + (",\n" + this.convert_sphere sphere_)
		)
		
		-- End
		return_ = return_ + "\n    ]\n" +
									"}"

		-- Return
		return_
	),
	
	fn save_to =
	(
		-- Prompt file
		filepath = getSaveFileName caption:"Export Spheres and Cameras"  filename:this.last_saved_filepath types:"JSON (.json)|*.json"
		if filepath != undefined do
		(
			this.last_saved_filepath = filepath
			file = createFile filepath
			content = this.convert()
			format (content) to:file
			close file
		)
	)
)

FrangiRayConverter = FrangiRayConverter()

macroScript FrangiRay_SaveTo
	category:"#FrangiRay"
	toolTip:"FrangiRay convert scene"
(
	FrangiRayConverter.save_to()
)
