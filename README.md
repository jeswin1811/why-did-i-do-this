# 🧠💥 *C-ing Is Believing* – A Barebones Web Server in C on Windows.

> *"Boredom fuels creativity. Or insanity. I forget which."*  
> — Me, while writing this server.

## 🚀 About

One lazy day, I randomly remembered a convo with a friend about people writing web servers in C.  
Everyone builds them on Linux. So naturally, I decided to go full rogue and write one **on Windows**, using good ol’ Winsock2.

Was there a reason? Not really.  
Did anyone ask for this? Nope.  
But did I do it anyway? Heck yes.

Welcome to my tiny HTTP server written from scratch in C that serves static HTML pages — just because I could.

## ⚙️ Features

- ✅ **Understands basic HTTP GET requests**  
  Just enough to serve `/` or `/index.html`, and nothing else. Like a puppy that knows exactly one trick.

- 🧱 **Raw Winsock2-based TCP server**  
  Zero frameworks. Just C, sockets, and a burning desire to make something unnecessarily hard.

- 📄 **Serves static HTML files**  
  Specifically `static/index.html`. That’s it. No routing, no templating, no nonsense.

- 🧍 **Single-client mode only**  
  Think of it as a one-person restaurant: great service, but only *one customer* at a time.

- 🧯 **Fails gracefully (most of the time)**  
  Can return a lovely handcrafted `404 Not Found` page if the file isn't there or something breaks.

## 🛠️ Setup

> **🪟 Requires:** Windows + a C compiler like `gcc` (via MinGW)

1. Clone this repo (or just grab `main.c`)
2. Create a folder named `static/` and drop your `index.html` in there
3. Compile the code:
   ```bash
   gcc main.c -o webserver.exe -lws2_32
   ```
4. Run the server:
   ```bash
   ./webserver.exe
   ```
5. Open your browser and go to [http://localhost:8080](http://localhost:8080)

## 🧪 Demo Behavior

| Request               | Response                          |
|----------------------|-----------------------------------|
| `/` or `/index.html` | 200 OK + contents of `index.html` |
| Anything else        | 404 Not Found                     |

## 🤖 What It *Doesn't* Do (Yet)

- Handle multiple simultaneous clients  
- Serve different routes or file types  
- Support HTTPS, headers, or advanced HTTP features  
- Work on Linux (this one's a Windows rebel)

But hey — it speaks HTTP, it delivers HTML, and it works. That’s a win.

## 📓 Why Though?

I don’t really know.  
Sometimes your brain just whispers “what if you built a web server in raw C for no reason?” and you listen.

Also, writing something as low-level as a web server reminds you how much we take things like Flask, Express, or Apache for granted. 😅

## 🧠 Final Thoughts

This won't scale.  
It won't secure your startup.  
But it *will* teach you how a simple HTTP server works under the hood.
