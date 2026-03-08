extends Node


var left = { "monks": 3, "demons": 3 }
var right = { "monks": 0, "demons": 0 }

var boat_side = "left"
var boat_passengers = []

@onready var left_bank = $"../LeftBank"
@onready var right_bank = $"../RightBank"
@onready var boat = $"../Boat"


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for character in $"../LeftBank".get_children():
		character.clicked.connect(_on_character_clicked)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _on_character_clicked(character):
	print("Clicked:", character)
