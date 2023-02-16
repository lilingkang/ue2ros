import unreal


def my_function():
    unreal.log('This is an Unreal log')
    unreal.log_error('This is an Unreal error!!!')
    unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.EditorAssetLibrary.load_blueprint_class('/Game/MyBlueprint.MyBlueprint_C'), unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    unreal.EditorLevelLibrary.spawn_actor_from_object(unreal.EditorAssetLibrary.load_asset('/Game/MyActor.MyActor'), unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
