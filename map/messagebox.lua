--[[
    about messagebox.lua

    convienence functions for the message box C api
]]

messagebox = 
{
    text = nil --the c pointer
}

function messagebox:new(text)
    o = {};
    setmetatable(o, self);
    self.__index = self;

    o.text = text;
    message_box_create(text);

    return o;
end