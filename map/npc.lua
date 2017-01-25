require "input"

--[[

    about npc.lua

    potentially (more than likely) some NPC related things.

]]

local kNpcRange = 32;

local function isNPCInRange(npc, range)
  assert(npc and range, "npc and range must not be nil or 0!");

  player_x = player_get_x(); --c function
  player_y = player_get_y();

  npc_x, npc_y = npc:get_position();

  min_x = npc_x - range
  min_y = npc_y - range

  max_x = npc_x + range
  max_y = npc_x + range

  if((player_x > min_x and player_x < max_x) and (player_y > min_y and player_y < max_y)) then
    return true;
  end

  return false;
end

--the NPC template
NPC = {
    name = "Tester",
    sprite = nil, --C pointer to the sprite_t data
    interact_function = nil --function to call upon interaction
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

function NPC:interact()
    if(self.interact_function ~= nil) then
        if(isNPCInRange(self, kNpcRange) and input_is_button_down(BUTTON_INTERACT) == 1) then
            self.interact_function(self);
        end
    end
end

function NPC:set_interact_action(interact_action)
    self.interact_function = interact_action;
end

function NPC:update()
    sprite_update(self.sprite);
    self:interact();
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

function NPC:get_position()
    return sprite_get_x(self.sprite), sprite_get_y(self.sprite);
end

function NPC:destroy()
    if(self.sprite ~= nil) then
        sprite_destroy(self.sprite);
    end
    self.name = nil;
end