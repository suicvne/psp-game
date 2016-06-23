originX = 10;
counter = 0;

function onLoad(map) --map contains table based metadata for the level
end

function onUpdate()
  --print("Hello world!");
  if(counter < 2000) then
    counter = counter + 1;
  end
end

function onDraw()
  draw_text("send nudes", 10, 22);
  draw_text("send nudes", 10, 32);
  draw_text("send nudes", 10, 42);
  draw_text("send nudes", 10, 52);
  draw_text("send nudes", 10, 62);
  draw_text("send nudes", 10, 72);
  --if(counter > 100) then
    --x = (-(counter * 2 / originX));
    --draw_text("Welcome to the Jungle", x, 52);
  --else
--    draw_text("Welcome to the Jungle", originX, 52);
  --end

  --draw_text("counter: " .. counter, 10, 22);
end
