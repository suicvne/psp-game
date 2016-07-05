local kMinutesPerHour = 60;
local kHoursPerDay = 12;
local kTicksPerSecond = 120; --120

time_hour = 0;
time_minute = 0;
time_ticks = 0;

local kMinLighting = 0.001;
local kMaxLighting = 1.0;

function time_loop()
  time_ticks = time_ticks + 1;
  if(time_ticks == kTicksPerSecond) then
    time_minute = time_minute + 1;
    if(time_minute == kMinutesPerHour) then
      time_hour = time_hour + 1;
      time_minute = 0;
    end

    if(time_hour == kHoursPerDay) then
      time_hour = 0;
    end

    time_ticks = 0;
  end
end

--[[
function calculateLighting()
  time = (hour + (minute / kMinutesPerHour)) / kHoursPerDay;
  light = kMaxLighting * math.sin(2 * math.pi * time);
  return math.clamp(light, kMinLighting, kMaxLighting);
end
]]