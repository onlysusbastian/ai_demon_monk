extends Node2D


@onready var left_land  = $environmen/left_land
@onready var right_land = $environmen/right_land
@onready var boat_area  = $boat/CharacterBody2D/boat_body 

@onready var left_spawn  = $environmen/left_land/left_spawn
@onready var right_spawn = $environmen/right_land/right_spawn
@onready var boat_body   = $boat/CharacterBody2D
var spawn_height = 150
var character_area = {}

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.

func _process(_delta: float) -> void:
	pass


func _on_left_spawn_body_entered(body: Node2D) -> void:
	character_area[body] = "LEFT"

func _on_right_spawn_body_entered(body: Node2D) -> void:
	character_area[body] = "RIGHT"

func _on_boat_body_body_entered(body: Node2D) -> void:
	character_area[body] = "BOAT"


func _on_demon_button_pressed() -> void:
	var boat_pos = boat_body.global_position
	var character = $characters/demon
	character.global_position = boat_pos - Vector2(0, spawn_height)

	#character.on_boat = false
	pass # Replace with function body.
