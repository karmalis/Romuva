function init()
    print(ECS)

    --- "data/models/monkey.obj", {x = 0; y = 0; z = -2.0}
--    local monkeyShader = Video.Shaders.Manager.Create("monkey_shader", "data/shaders/basic_vs.glsl", "data/shaders/basic_fs.glsl")
--    local guiShader = Video.Shaders.Manager.Create("gui_shader", "data/shaders/2d_vs.glsl", "data/shaders/2d_fs.glsl")
--
--    local monkey = ECS.Manager.Entity.Create("monkey")
--    monkey.addComponent("mesh", { model = "data/models/monkey.obj" })
--    monkey.addComponent("transform", { x = 0; y = 0; z = -2.0 })
--
--    local camera = ECS.Manager.Entity.Create("camera")
--    camera.addComponent("transform", { x = 0, y = 0; z = -2.0 })
--    camera.addComponent("camera", {
--        aspect = 1270 / 720;
--        near = 0.1;
--        far = 100.0;
--        fov = 70.0;
--    })
end

function update(delta, bravo)
end

