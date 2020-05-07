[Unit]
Description=CorsairOne daemon
Documentation=https://github.com/SBAM/corsair_one_d
StopWhenUnneeded=true

[Service]
Type=simple
ExecStart=@CMAKE_INSTALL_FULL_BINDIR@/corsair_one_d --daemon
Restart=on-failure
SuccessExitStatus=0
RestartPreventExitStatus=2
