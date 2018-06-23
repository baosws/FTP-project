# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions
alias py3='python3'
alias py2='python2'
alias sccnf='sudo cvt 1920 1080 60 && sudo xrandr --newmode "1920x1080_60.00" 173.00 1920 2048 2248 2576 1080 1083 1088 1120 -hsync +vsync && sudo xrandr --addmode eDP-1-1 1920x1080_60.00'


function foo() {
    if [[ $1 == *.cpp ]]; then
        cmd="g++ -std=c++17 '$1' -o '${1%%.*}' && time ./'${1%%.*}'"
    elif [[ $1 == *.py ]]; then
        cmd="time python3 '$1'"
    elif [[ $1 == *.java ]]; then
        cmd="javac '$1' && time java '${1%%.*}'"
    fi

    if ! [[ -z $2 ]]; then
        cmd=$cmd" < '$2'"
    fi
    if ! [[ -z $3 ]]; then
        cmd=$cmd" > '$3'"
    fi
    
    eval $cmd
}
reset=$(tput sgr0)
bold=$(tput bold)
black=$(tput setaf 0)
red=$(tput setaf 1)
green=$(tput setaf 2)
yellow=$(tput setaf 3)
blue=$(tput setaf 4)
magenta=$(tput setaf 5)
cyan=$(tput setaf 6)
white=$(tput setaf 7)
user_color=$green
[ "$UID" -eq 0 ] && { user_color=$red; }
PS1="\[$reset\][\[$cyan\]\A\[$reset\]]\[$user_color\]\u@\h(\l)\
\[$white\]:\[$blue\]\W\[$reset\][\[$yellow\]\$?\[$reset\]]\[$white\]\
\\$\[$reset\] "
export PATH="$PATH:/opt/mssql-tools/bin"
