## Safe Scripting the Tilde Way

Welcome to tilde.club. For many of you this may be your first multi-user host and for even more of you this may be your *first* host. Welcome to all of you.  

Whether you are used to Unix, Linux and programming or a complete beginner please consider this an invitation to create and build new things. 

It's important when creating however that we respect the shared environment and that we respect the boundaries of others.

As Paul says:

> no drama. be respectful. have fun.

What does that mean with respect to scripting and programming on a shared host?

- DOs
    - Do respect the shared CPU/Disk/Network resources
    - Do keep things that require a tilde.club login off the internet:
        - Finger info
        - .plan and .profile files
        - local home directory files
        - Note: Exceptions could include an opt-in file or special permissions from the user.
    - Only scan homedirs with a .public file
    - *What about `public_html` dirs? -- mc*

- DON'Ts
    - Don't look at others mail files or .ssh/ dir 
    - Do not attempt to circumvent file and directory permissions or logins

In short use your common sense and consider how your actions may affect others.
  
If you are unsure if something is a good idea or not, head to irc or message using the `wall` command and ask others what they think.  

If you want a conclusive answer, contact one of the system operators.

#### Ways we can help others stay private

Because we generally have access across the server and there are quite a few users, we may not know who is willing share their data.

##### The `~/.public` file
In order to be sure, any code that looks into a user homedir will check for the existence of a `~/.public` file before going further.  If the file doesn't exist that directory will be skipped.  

The way this would work in bash would be:

    if [ -f $userdir/.public ]
    then 
        command $userdir | process >> outputfile
    else
        next
    fi

Other scripting examples provided upon request (please submit)


##### Thinking privacy

Consider the source of the data.

E.g. We know that finger data might contain personal data such as phone numbers, and other identifying information and is not generally available without a tilde.club login. 

Before exposing data such as this to the world, it should check for an opt in file such as the .public file file test before presenting info to the outside world. 
*(should ~/.public also signify stuff like this or something else? can it be an ad-hoc opt-in file? --mc)*


#### Shared Resources

- In general
    - Consider executing long running processes during overnight hours when fewer users are on the system
- CPU
    - Use the `nice` command to keep intensive processes from affecting others
        - The `nice -n19 -p$$` placed in a script will make sure it runs at the lowest priority.
        - Run a long command at the lowest CPU priority: `nice -n 19 ~/bin/command`
        - Change the priority of process 923 (also known as "re-nicing a process") : `renice -17 -p923`
- Disk
    - On any commands that will heavily use disk, consider using the `ionice` command so that scripts will not affect interactive users
        - By adding the `ionice -c3 -p$$` command to any script, it will only use disk when idle.
        - You can also run a command or script: `ionice -c3 -t ~/bin/command`  
