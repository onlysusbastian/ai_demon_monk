extends CharacterBody2D

signal clicked(character)

var location = "left"
var character_type = "monk"

func _ready():
	input_pickable = true

func _input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton and event.pressed:
		emit_signal("clicked", self)
