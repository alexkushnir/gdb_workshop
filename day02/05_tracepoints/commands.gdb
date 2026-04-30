tfind start
while ($trace_frame != -1)
tdump
tfind
end

while ($trace_frame != -1)
pipe tdump | cat >> output.txt
tfind
end
