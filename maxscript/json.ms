-- Structure that mimics Python's dict, and allows recursive (de)serialization to JSON files
-- Rewrite from C# to MaxScript of Patrick van Bergen's :
-- http://techblog.procurios.nl/k/news/view/14605/14863/how-do-i-write-my-own-parser-(for-json).html
-- ---------------------------------
-- v.moriceau@cube-creative.com
-- 23/01/2015
-- ---------------------------------
struct dict
(
	-- User Data
	_keys = #(),
	_values = #(),
	
	fn keys =
	(
		this._keys
	),

	fn values =
	(
		this._values
	),
	
	-- Sets a pair, add key of not exists, overwrite value if key exists
	fn set_pair pair_key pair_value =
	(
		-- Get Index in keys
		key_index = findItem this._keys pair_key
		-- If key already exists
		if key_index != 0 then
		(
			-- Replace Content
			this._values[key_index] = pair_value
		)
		-- If root doesn't exists
		else
		(
			-- Add to self.roots
			append this._keys pair_key
			-- Recurse
			this.set_pair pair_key pair_value
		)
	),
	
	-- Gets a pair's value from it's key
	fn get_pair pair_key =
	(
		-- Get Index in keys
		key_index = findItem this._keys pair_key
		-- If not 0
		if key_index != 0 do
		(
			-- Return Content
			return this._values[key_index]
		)
	)
)
	
struct json
(
	-- Tokens
	_TOKEN_NONE = 0,
	_TOKEN_CURLY_OPEN = 1,
	_TOKEN_CURLY_CLOSE = 2,
	_TOKEN_SQUARED_OPEN = 3,
	_TOKEN_SQUARED_CLOSE = 4,
	_TOKEN_COLON = 5,
	_TOKEN_COMMA = 6,
	_TOKEN_STRING = 7,
	_TOKEN_NUMBER = 8,
	_TOKEN_TRUE = 9,
	_TOKEN_FALSE = 10,
	_TOKEN_NULL = 11,
	-- Parser data
	_parser_index = 0,
	_parser_success = True,
	_parser_file_content = StringStream "",
	_parser_file_length = 0,

	-- Recursively Serializes data into given filepath
	fn serialize dict_ filepath =
	(
		-- Create Folders
		makeDir (getFilenamePath filepath)
		-- Open file for writing
		file = createFile filepath
		-- If Success
		if file != undefined do
		(
			-- Serialize Self
			serialized = this._serialize_value dict_
			-- Write to file
			format serialized to:file
			-- Close file
			close file
			-- Ok for listener
			ok
		)
	),
	
	-- Recursively Serializes and returns string
	fn serialize_string dict_ =
	(
		-- Serialize Self
		serialized = this._serialize_value dict_
	),
	
	-- Recursively Deserializes data from given filepath
	fn deserialize filepath =
	(
		-- Open file for reading
		file = openFile filepath
		-- If Success
		if file != undefined do
		(
			-- Prepare Stream
			this._parser_file_content = StringStream ""
			this._parser_file_length = 0
			-- Read Each Char
			while not eof file do
			(
				-- Read
				format (readChar file) to:this._parser_file_content
				-- Increment Length
				this._parser_file_length = this._parser_file_length + 1
			)
			-- Close File
			close file
			-- If Anything in file
			if this._parser_file_content != undefined do
			(
				-- Init
				this._parser_index = 0
				this._parser_success = True
				-- Begin Deserialization
				return_value = this._deserialize_value()
				-- Free stream
				free this._parser_file_content
				-- Return
				return return_value
			)
		)
	),
	

	-- Recursively Deserializes data from given filepath
	fn deserialize_string string_ =
	(
		-- Prepare Stream
		this._parser_file_content = StringStream string_
		-- If Anything in file
		if this._parser_file_content != undefined do
		(
			-- Init
			this._parser_file_length = string_.count
			this._parser_index = 0
			this._parser_success = True
			-- Begin Deserialization
			return_value = this._deserialize_value()
			-- Free stream
			free this._parser_file_content
			-- Return
			return return_value
		)
	),

	-- Deserializes a value
	fn _deserialize_value =
	(
		-- Next Token
		next_token = this._next_token()
		-- Case
		case next_token of
		(
			(this._TOKEN_STRING) : ( return this._deserialize_string() )
			(this._TOKEN_NUMBER) : ( return this._deserialize_number() )
			(this._TOKEN_CURLY_OPEN) : ( return this._deserialize_dict() )
			(this._TOKEN_SQUARED_OPEN) : ( return this._deserialize_array() )
			(this._TOKEN_TRUE) : ( return True )
			(this._TOKEN_FALSE) : ( return False )
			(this._TOKEN_NULL) : ( return undefined )
			(this._TOKEN_NONE) : ( return undefined )
		)
	),

	-- Deserializes a string
	fn _deserialize_string =
	(
		-- Init String
		string_ = ""
		complete = False
		-- While not complete
		while not complete do
		(
			-- Read
			char = this._read()
			this._index_inc()
			-- If end of Json
			if this._parser_index == this._parser_file_length do exit
			-- If Closing quote
			if char == "\"" then complete = True
			-- If not Closing Quote
			else
			(
				-- If Escaping backlash
				if char == "\\" then
				(
					-- Read
					char = this._read()
					-- If end of Json
					if this._parser_index == this._parser_file_length do exit
					-- If quote
					if char == "\"" do string_ = string_ + "\""
					-- If Backslash
					if char == "\\" do string_ = string_ + "\\"
					-- If Slash
					if char == "/" do string_ = string_ + "/"
					-- If Return
					if char == "n" do string_ = string_ + "\n"
					-- If Return
					if char == "r" do string_ = string_ + "\r"
					-- If Tab
					if char == "t" do string_ = string_ + "\t"
					-- Next Index
					this._index_inc()
				)
				-- No Escaping
				else
				(
					-- Simply Append
					string_ = string_ + char
				)
			)
		)
		-- Return
		return string_
	),
	
	-- Deserializes a number
	fn _deserialize_number =
	(
		-- Go Back one char
		this._index_dec()
		-- Init String
		number_ = ""
		complete = False
		-- While Numeric Chars
		while not complete do
		(
			-- Read
			char = this._read()
			this._index_inc()
			-- If numercial char
			if char == "0" or char == "1" or char == "2" or char == "3" or char == "4" or char == "5" or char == "6" or char == "7" or char == "8" or char == "9" or char == "+" or char == "-" or char == "." or char == "E" then
			(
				-- Append
				number_ = number_ + char
			)
			-- If not numerical character
			else 
			(
				-- Complete
				complete = True
				-- Go Back one char
				this._index_dec()
			)
			-- If end of Json
			if this._parser_index == this._parser_file_length do exit
		)
		-- Return Converted
		return number_ as float
	),

	-- Deserializes a dict
	fn _deserialize_dict =
	(
		-- Init Dict
		dict_ = dict()
		complete = False
		-- While inside brackets
		while not complete do
		(
			-- Next Token
			token = this._next_token()
			-- None
			if token == this._TOKEN_NONE do return undefined
			-- Comma -> Next Token
			if token == this._TOKEN_COMMA do continue
			-- Close Bracket
			if token == this._TOKEN_CURLY_CLOSE do
			(
				-- Return
				return dict_
			)
			-- Key
			key_ = this._deserialize_string()
			-- Next Token
			token = this._next_token()
			-- If not Colon
			if token != this._TOKEN_COLON do return undefined
			-- Value
			value_ = this._deserialize_value()
			-- Set pair
			dict_.set_pair key_ value_
		)
		-- Return
		return dict_
	),

	-- Deserializes a array
	fn _deserialize_array =
	(
		-- Init Array
		array_ = #()
		complete = False
		-- While inside brackets
		while not complete do
		(
			-- Next Token
			token = this._next_token()
			-- Coma
			if token == this._TOKEN_COMMA do continue
			-- None
			if token == this._TOKEN_NONE do return undefined
			-- Close Bracket
			if token == this._TOKEN_SQUARED_CLOSE do return array_
			-- Go Back one char
			this._index_dec()
			-- Item
			item_ = this._deserialize_value()
			-- Append
			append array_ item_
		)
		-- Return
		return array_
	),
	
	-- Reads until next token
	fn _next_token =
	(
		-- Skip White Spaces
		this._skip_whitespaces()
		-- If index is last char
		if this._parser_index >= this._parser_file_length do
		(
			-- Return None
			return this._TOKEN_NONE
		)
		-- Get Token
		token = this._read()
		-- Next Position
		this._index_inc()
		-- Case
		case token of
		(
			"{" : (return this._TOKEN_CURLY_OPEN)
			"}" : (return this._TOKEN_CURLY_CLOSE)
			"[" : (return this._TOKEN_SQUARED_OPEN)
			"]" : (return this._TOKEN_SQUARED_CLOSE)
			"," : (return this._TOKEN_COMMA)
			"\"" : (return this._TOKEN_STRING)
			"0" : (return this._TOKEN_NUMBER)
			"1" : (return this._TOKEN_NUMBER)
			"2" : (return this._TOKEN_NUMBER)
			"3" : (return this._TOKEN_NUMBER)
			"4" : (return this._TOKEN_NUMBER)
			"5" : (return this._TOKEN_NUMBER)
			"6" : (return this._TOKEN_NUMBER)
			"7" : (return this._TOKEN_NUMBER)
			"8" : (return this._TOKEN_NUMBER)
			"9" : (return this._TOKEN_NUMBER)
			"-" : (return this._TOKEN_NUMBER)
			":" : (return this._TOKEN_COLON)
		)
		-- Previous Position
		this._index_dec()
		-- Remaining Length
		remaining_length = this._parser_file_length - this._parser_index
		-- false
		if remaining_length >= 5 do
		(
			if this._read() == "f" and
			this._read_at this._parser_file_content (this._parser_index + 1) == "a" and
			this._read_at this._parser_file_content (this._parser_index + 2) == "l" and
			this._read_at this._parser_file_content (this._parser_index + 3) == "s" and
			this._read_at this._parser_file_content (this._parser_index + 4) == "e" do
			(
				-- Set index
				this._index_inc inc:5
				-- Return Token
				return this._TOKEN_FALSE
			)
		)
		-- true
		if remaining_length >= 4 do
		(
			if this._read() == "t" and
			this._read_at this._parser_file_content (this._parser_index + 1) == "r" and
			this._read_at this._parser_file_content (this._parser_index + 2) == "u" and
			this._read_at this._parser_file_content (this._parser_index + 3) == "e" do
			(
				-- Set index
				this._index_inc inc:4
				-- Return Token
				return this._TOKEN_TRUE
			)
		)
		-- nul
		if remaining_length >= 4 do
		(
			if this._read() == "n" and
			this._read_at this._parser_file_content (this._parser_index + 1) == "u" and
			this._read_at this._parser_file_content (this._parser_index + 2) == "l" and
			this._read_at this._parser_file_content (this._parser_index + 3) == "l" do
			(
				-- Set index
				this._index_inc inc:4
				-- Return Token
				return this._TOKEN_NULL
			)
		)
		-- Else, none
		return this._TOKEN_NONE
	),
	
	-- Increments index as long as whitespaces are encountered
	fn _skip_whitespaces =
	(
		-- Read Char
		char = this._read()
		-- As long as space, tab, carraige return
		while char == " " or char == "\t" or char == "\n" or char == "\r" do
		(
			-- Next Position
			this._index_inc()
			-- Read Char
			char = this._read()
		)
	),
	
	-- Recursively Serializes data from given object
	fn _serialize_value value_ indent_level:0 =
	(
		-- Init serialized string
		serialized = ""
		-- Get Class of value
		value_class = classOf value_
		-- If Dict
		if value_class == dict do
		(
			-- Init Dict
			serialized = serialized + (this._set_indentation "{\n" indent_level)
			-- To identify last item
			index = 1
			-- Each key of Dict
			for key_ in value_._keys do
			(
				-- Get Corresponding
				key_value = value_.get_pair key_
				-- Serialize Key
				key_ = this._serialize_value key_
				-- If before last item
				if index < value_._keys.count then
				(
					-- Suffix with coma
					suffix = ",\n"
				)
				else
				(
					-- Suffix without coma
					suffix = "\n"
				)
				-- Serialize with incremtal indent
				serialized = serialized + (this._set_indentation "" (indent_level + 1)) + key_ + ":" + (this._remove_first_tabs (this._serialize_value key_value indent_level:(indent_level + 1))) + suffix
				-- Increment Index
				index = index + 1
			)
			-- Close Array
			serialized = serialized + (this._set_indentation "}" indent_level)
		)
		-- If Array
		if value_class == Array do
		(
			-- Init Array
			serialized = serialized + (this._set_indentation "[\n" indent_level)
			-- To identify last item
			index = 1
			-- Each item of Array
			for value_item in value_ do
			(
				-- If before last item
				if index < value_.count then
				(
					-- Suffix with coma
					suffix = ",\n"
				)
				else
				(
					-- Suffix without coma
					suffix = "\n"
				)
				-- Serialize with incremtal indent
				serialized = serialized + (this._serialize_value value_item indent_level:(indent_level + 1) + suffix)
				-- Increment Index
				index = index + 1
			)
			-- Close Array
			serialized = serialized + (this._set_indentation "]" indent_level)
		)
		-- If String
		if value_class == String do
		(
		    -- Escape
		    value_ = substituteString value_ "\\" "\\\\"
		    value_ = substituteString value_ "\"" "\\\""
		    value_ = substituteString value_ "\n" "\\n"
		    value_ = substituteString value_ "\t" "\\t"
		    value_ = substituteString value_ "\t" "\\r"
			-- Serialize
			serialized = serialized + (this._set_indentation ("\"" + value_ + "\"") indent_level)
		)
		-- If Int or Float
		if value_class == Float or value_class == Integer do
		(
			-- Serialize
			serialized = serialized + (this._set_indentation (value_ as string) indent_level)
		)
		-- If Boolean
		if value_class == BooleanClass do
		(
			-- Serialize
			serialized = serialized + (this._set_indentation (value_ as string) indent_level)
		)
		-- If Undefined
		if value_class == UndefinedClass do
		(
			-- Serialize
			serialized = serialized + (this._set_indentation ("null") indent_level)
		)
		-- Return
		return serialized
	),

	-- Adds Tab at beggining of string
	fn _set_indentation string_ indentation_level =
	(
		-- Init Prefix
		prefix = ""
		-- Generate Indent
		for i = 1 to indentation_level do
		(
			prefix = prefix  + "    "
		)
		-- Return
		return prefix + (string_ as string)
	),
	
	-- Remove Tabsat beggining of string
	fn _remove_first_tabs string_ =
	(
		-- Each Tabulation
		while (substring string_ 1 1) == " " do
		(
			-- Remove Tab
			string_ = substring string_ 2 (string_.count - 1)
		)
		-- Return
		return string_
	),
	
	-- Reads a char at given index in given stringstream
	fn _read_at string_stream index =
	(
		-- Position cursor
		seek string_stream index
		-- If not EOF
		if not eof string_stream do
		(
			-- Read
			char = (readchar string_stream)
			-- Return
			return char
		)
	),
	
	-- Reads parser stringsrteam at parser index
	fn _read =
	(
		return (this._read_at this._parser_file_content this._parser_index)
	),
	
	-- Increments parser index
	fn _index_inc inc:1=
	(
		this._parser_index = this._parser_index + inc
	),
	
	-- Decrements parser index
	fn _index_dec dec:1=
	(
		this._parser_index = this._parser_index - dec
	)
)

-- Make Static
json = json()

/*
-- Example
-- Nested Dict
fn test =
(
	test_dict_01 = dict()
	test_dict_01.set_pair "scene_name" "Le nom de la scene"
	test_dict_01.set_pair "scene_id" 45687
	test_dict_01.set_pair "user_id" 408
	test_dict_01.set_pair "assets_ids" #(15486,574587,548613,65487,32186,1384)
	test_dict_01.set_pair "test_none" False
	-- Parent Dict
	test_dict_02 = dict()
	test_dict_02.set_pair "shot_id" 458772
	test_dict_02.set_pair "cam_fov" 63.752
	test_dict_02.set_pair "scene_dict" test_dict_01
	-- Serialization
	(json()).serialize test_dict_02 "D:\\mascript_serial.json"
	-- Deserialization
	test_dict_03 = (json()).deserialize "D:\\mascript_serial.json"
	-- Return
	return test_dict_03
)
format "==========================================="
print (test())
--*/