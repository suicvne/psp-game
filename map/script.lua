--package.path = "map/?.lua" --todo: map this from inside of C?
require "time"
require "player"

framecount = 0;

function onLoad() --map contains table based metadata for the level
  print("Hello from inside of Lua!");
  print("Map Name: " .. tilemap_get_name(_current_tilemap));
  print("Map Size: " .. tilemap_get_width(_current_tilemap) .. " x " .. tilemap_get_height(_current_tilemap));

  message_box_set_message("This is a much longer message detailing a quest\nor something that you have to do for some\nperson.");
end

function math.clamp(value, lower, upper)
  assert(value and lower and upper, "must have all values present for clamp.");
  if (lower > upper) then
    lower, upper = upper, lower
  end
  return math.max(lower, math.min(upper, value));
end

function onUpdate()
  framecount = framecount + 1;
  
  if(framecount == 1) then
    message_box_set_message("Welcome to the Rock Bottom alpha!\nYou can't do much, sorry!");
    message_box_set_visibility(true);
  elseif(framecount == 2) then
    message_box_set_message("There is, however, a test of changing tiles. :)")
    message_box_set_visibility(true);
  end

  time_loop();
  check_input(_current_tilemap);
end

function onDraw()
  message = string.format("time: %d:%02d", time_hour, time_minute, time_ticks);
  draw_text(message, 10, 32);
  message = string.format("energy: %d", player_current_energy);
  draw_text(message, 10, 42);
  
  get_direction_string(); --why is this necessary
  
  value = input_is_button_down(BUTTON_USE);
  if(value == 1) then
    message = "interact: yes";
  else
    message = "interact: no";
  end
  
  draw_selection();
end
