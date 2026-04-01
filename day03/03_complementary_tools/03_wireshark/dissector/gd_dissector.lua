-- GD Custom Protocol Wireshark Dissector
-- Defines dissector to parse TCP traffic on port 9999

-- Declare the protocol
local gd_proto = Proto("gd_protocol", "GD Custom Protocol")

-- Declare protocol fields
-- Header fields (8 bytes total)
local f_magic       = ProtoField.string("gd_protocol.magic", "Magic Bytes")
local f_msg_type    = ProtoField.uint8("gd_protocol.msg_type", "Message Type", base.DEC, { [1] = "Request", [2] = "Response" })
local f_reserved    = ProtoField.uint8("gd_protocol.reserved", "Reserved", base.HEX)
local f_payload_len = ProtoField.uint32("gd_protocol.payload_len", "Payload Length", base.DEC)

-- Request body fields (Type 1)
local f_req_client_id = ProtoField.uint32("gd_protocol.request.client_id", "Client ID", base.DEC)
local f_req_version   = ProtoField.uint16("gd_protocol.request.version", "Protocol Version", base.DEC)
local f_req_message   = ProtoField.string("gd_protocol.request.message", "Request Message")

-- Response body fields (Type 2)
local f_res_status    = ProtoField.uint16("gd_protocol.response.status_code", "Status Code", base.DEC)
local f_res_message   = ProtoField.string("gd_protocol.response.message", "Response Message")

-- Register fields
gd_proto.fields = {
    f_magic, f_msg_type, f_reserved, f_payload_len,
    f_req_client_id, f_req_version, f_req_message,
    f_res_status, f_res_message
}

-- Define the dissector function
function gd_proto.dissector(buffer, pinfo, tree)
    local length = buffer:len()
    
    -- Minimum size is 8 bytes (Header)
    if length < 8 then return end

    -- Check for magic bytes to ensure this is our protocol
    local magic = buffer(0, 2):string()
    if magic ~= "GD" then return end

    -- Set protocol column in packet list
    pinfo.cols.protocol = "GD"

    -- Add the main protocol tree
    local subtree = tree:add(gd_proto, buffer(), "GD Custom Protocol Data")
    
    -- Process the Header
    local msg_type = buffer(2, 1):uint()
    local payload_len = buffer(4, 4):uint()
    
    local type_str = "Unknown"
    if msg_type == 1 then
        type_str = "Request"
    elseif msg_type == 2 then
        type_str = "Response"
    end
    
    -- Set the Info column to something helpful
    pinfo.cols.info = type_str .. " Message (Len: " .. payload_len .. ")"

    -- Add header fields
    local header_tree = subtree:add(buffer(0, 8), "Header")
    header_tree:add(f_magic, buffer(0, 2))
    header_tree:add(f_msg_type, buffer(2, 1))
    header_tree:add(f_reserved, buffer(3, 1))
    header_tree:add(f_payload_len, buffer(4, 4))
    
    -- Process Payload Body
    if payload_len > 0 and length >= 8 + payload_len then
        local body_tree = subtree:add(buffer(8, payload_len), "Body (" .. type_str .. ")")
        
        if msg_type == 1 then -- Request body parsing
            if payload_len >= 6 then
                body_tree:add(f_req_client_id, buffer(8, 4))
                body_tree:add(f_req_version, buffer(12, 2))
                
                local msg_len = payload_len - 6
                if msg_len > 0 then
                    body_tree:add(f_req_message, buffer(14, msg_len))
                end
            end
        elseif msg_type == 2 then -- Response body parsing
            if payload_len >= 2 then
                body_tree:add(f_res_status, buffer(8, 2))
                
                local msg_len = payload_len - 2
                if msg_len > 0 then
                    body_tree:add(f_res_message, buffer(10, msg_len))
                end
            end
        end
    end
end

-- Register dissector to port 9999
local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(9999, gd_proto)
