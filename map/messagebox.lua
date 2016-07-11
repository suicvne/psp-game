--[[
    about messagebox.lua

    convienence functions for the message box C api
]]

messagebox = {}

function messagebox.show_message(text)
    message_box_set_message(text);
    message_box_set_visibility(true);

--[[
    co = coroutine.create(
        function()
            while message_box_is_visible() == true do
                message_box_update();
                message_box_draw();

                if(input_is_button_down(BUTTON_INTERACT)) then
                    message_box_set_visibility(false);
                end
            end
        end
    );
    coroutine.resume(co);
]]

end