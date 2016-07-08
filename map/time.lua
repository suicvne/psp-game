local kMinutesPerHour = 60;
local kHoursPerDay = 12;
local kTicksPerSecond = 90; --120

time_day = 1;
time_hour = 6; --default 6 
time_minute = 0; --default 0
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
      time_day = 1;
    end

    time_ticks = 0;
  end
end

--TODO: alpha blended lighting stuff

--[[
function calculateLighting()
  time = (hour + (minute / kMinutesPerHour)) / kHoursPerDay;
  light = kMaxLighting * math.sin(2 * math.pi * time);
  return math.clamp(light, kMinLighting, kMaxLighting);
end
]]