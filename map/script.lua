OSL_FX_NONE=0
OSL_FX_FLAT = 1
OSL_FX_ALPHA = 2
OSL_FX_ADD = 3
OSL_FX_SUB = 4
OSL_FX_RGBA = 0x100
OSL_FX_COLOR = 0x1000

kMinutesPerHour = 60;
kHoursPerDay = 12;
kTicksPerSecond = 1; --120

hour = 0;
minute = 0;
ticks = 0;

kMinLighting = 0.001;
kMaxLighting = 1.0;

function onLoad(map) --map contains table based metadata for the level
end

function math.clamp(value, lower, upper)
  assert(value and lower and upper, "must have all values present for clamp.");
  if (lower > upper) then
    lower, upper = upper, lower
  end
  return math.max(lower, math.min(upper, value));
end

function calculateLighting()
  time = (hour + (minute / kMinutesPerHour)) / kHoursPerDay;
  light = kMaxLighting * math.sin(2 * math.pi * time);
  return math.clamp(light, kMinLighting, kMaxLighting);
end

function onUpdate()
  ticks = ticks + 1;
  if(ticks == kTicksPerSecond) then
    minute = minute + 1;
    if(minute == kMinutesPerHour) then
      hour = hour + 1;
      minute = 0;
    end

    if(hour == kHoursPerDay) then
      hour = 0;
    end

    ticks = 0;
  end
end

function onDraw()

  --draw_set_alpha_color(RGBA(255, 127, 0));

  --draw_filled_rect(0, 0, 240, 272, RGBA(255, 255, 255, 255));

  --draw_set_alpha(OSL_FX_COLOR, 0);

  message = string.format("time: %d:%02d", hour, minute, ticks);
end
