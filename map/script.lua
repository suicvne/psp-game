--package.path = "map/?.lua" --todo: map this from inside of C?
require "time"
require "player"

function onLoad() --map contains table based metadata for the level
  print("Hello from inside of Lua!");
  print("Map Name: " .. tilemap_get_name(_current_tilemap));
  print("Map Size: " .. tilemap_get_width(_current_tilemap) .. " x " .. tilemap_get_height(_current_tilemap));
end

function math.clamp(value, lower, upper)
  assert(value and lower and upper, "must have all values present for clamp.");
  if (lower > upper) then
    lower, upper = upper, lower
  end
  return math.max(lower, math.min(upper, value));
end

function onUpdate()
  time_loop();
  check_input(_current_tilemap);
end

function onDraw()
  message = string.format("time: %d:%02d", time_hour, time_minute, time_ticks);
  draw_text(message, 10, 32);
  message = string.format("player: %d, %d", player_get_x(), player_get_y());
  draw_text(message, 10, 42);
  message = string.format("player direction: %s", get_direction_string());
  draw_text(message, 10, 52);
  
  value = input_is_button_down(BUTTON_USE);
  if(value == 1) then
    message = "interact: yes";
  else
    message = "interact: no";
  end
  draw_text(message, 10, 62);
  
  draw_selection();
end
