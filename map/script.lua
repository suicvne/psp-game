ingame_time = 0;
kMaxTime = 24000;
afternoon = false;

--0 is 6am
--6000 is noon (12)
--12000 is 6pm

function onLoad(map) --map contains table based metadata for the level
end

function getHour(time)
  return (time + 6000) / 1000
end

function onUpdate()
  ingame_time = ingame_time + 1;

  if(ingame_time == 12000) then
    afternoon = true;
  end

  if(ingame_time == 18000) then
    afternoon = false;
  end

  if(ingame_time == kMaxTime) then
    ingame_time = 0;
  end
end

function onDraw()
  suffix = "am";

  if(afternoon == true) then
    suffix = "pm";
  end

  message = string.format("time: %.2f %s", getHour(ingame_time), suffix);
  draw_text(message, 10, 22);
end
