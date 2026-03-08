extends CharacterBody2D


const SPEED = 300.0
const JUMP_VELOCITY = -400.0
var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")
var on_boat = false

func _physics_process(delta: float) -> void:
	
	if on_boat:
		velocity = Vector2.ZERO
		return
	
	print("Y:", global_position.y)

	if not is_on_floor():
		velocity.y += 1000 * delta

	move_and_slide()
