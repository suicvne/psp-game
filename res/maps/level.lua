--[[

  about <levelname>.lua

  the current iteration of the engine will load and execute a lua script with the same name as the level filename if it finds one
  the engine's lua interpreter will execute 3 functions at certain times and will skip them if it can't find them or they don't exist.

  1. onLoad() - called when the map is loaded. do initialization here.
  2. onUpdate() - called when the map's update function is called n times per second.
  3. onDraw() - called when the map's draw function is called n times per second.

  in the case of onUpdate() and onDraw(), both are called **AFTER** the map's normal drawing/updating functions.

]]

require "time" --time.lua contains functions for time related stuff. what else did you expect?
require "player" --lua testing of player functions (tilling soil at the moment)
require "npc"
require "messagebox"

local framecount = 0;

local npc_test_2;


function onLoad()
  npc_test_2 = NPC:new{name = "DJ Khaled", sprite = sprite_create("./res/ness.png")}

  npc_test_2:set_interact_action(
    function(npc)
      message_box_set_message("ok here take this red-bull.");
      message_box_set_visibility(true);
      player_current_energy = 110;
    end
  );

  tilemap_set_tile(_current_tilemap, 32, 32, 4);

  print('created test npc');

  --Setting the tile that the NPC sits on to a special one to see the grid
  --npc_test_2:destroy();

  --print("Hello from inside of Lua!");
  --print("Map Name: " .. tilemap_get_name(_current_tilemap));
  --print("Map Size: " .. tilemap_get_width(_current_tilemap) .. " x " .. tilemap_get_height(_current_tilemap));
end

function math.clamp(value, lower, upper)
  assert(value and lower and upper, "must have all values present for clamp.");
  if (lower > upper) then
    lower, upper = upper, lower
  end
  return math.max(lower, math.min(upper, value));
end

function onUpdate()
 --message_box_set_message("message w no specials");
 --message_box_set_visibility(true);
  npc_test_2:update(); --also calls to check for interaction

  time_loop();
  check_input(_current_tilemap);
end

function onDraw()

  npc_test_2:draw();

  message = string.format("Day %d - %d:%02d", time_day, time_hour, time_minute);
  draw_text(message, 10, 22);
  message = string.format("energy: %d", player_current_energy);
  draw_text(message, 10, 32);

  get_direction_string(); --why is this necessary

  draw_selection();
end
