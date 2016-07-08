--[[

    about npc.lua

    potentially some NPC related things.

]]

NPC = {
    name = "Tester",
    sprite = nil, --C pointer to the sprite_t data
};

function NPC:new(o)
    o = o or {};
    setmetatable(o, self);
    self.__index = self;

    sprite_set_frame_count(o.sprite, 2);
    sprite_set_frame_time(o.sprite, 10); --idk

    sprite_set_position(o.sprite, 32, 32);
    sprite_set_size(o.sprite, 15, 24);

    return o;
end

function NPC:update()
    sprite_update(self.sprite);
end

function NPC:draw()
    map_draw_sprite(self.sprite);
end

function NPC:set_name(name)
    self.name = name;
end

function NPC:set_position(x, y)
    sprite_set_position(self.sprite, x, y);
end

function NPC:destroy()
    if(self.sprite ~= nil) then
        sprite_destroy(self.sprite);
    end
    self.name = nil;
end