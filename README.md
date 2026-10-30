# Ultimate Warcraft 3 Next Generation (UWC3NG)

UWC3NG is an AMX Mod X plugin for Counter-Strike 1.6 by DA ("DarkAngel", Michel Vocks). In his words:

> Ultimate Warcraft 3 Next Generation (UWC3NG) is a raceless version of the Warcraft3:FT plugin you
> may be familiar with. UWC3NG provides players with 50 playable levels and a freeform skills system
> which allows you to mix and match skills of your choosing. In addition UWC3NG offers players a
> choice of 10 ultimate skills and you can train up to 3 different ultimates, as well as other
> special abilities. UWC3NG also offers an optional Enhancement system to provide your character
> with even more customization.
>
> UWC3NG is based on the new WC3:FT r11 plugin. It's also completely new rewritten!

It is a new mod, not a continuation of K2's [UWC3](https://github.com/wc3mods/uwc3) code. (Marticus'
2006 Subversion repository for UWC3 1.6.3 was also named `uwc3ng`; that is unrelated.)

## History

Each surviving release is one commit by DA, dated to its release, and tagged:

| Tag | Released | Source |
|---|---|---|
| `v1.4` | 2008-07-07, rebuilt 2008-07-18 | First public release, on forums.wc3mods.net (t=3309) and AlliedModders (t=73822). Recovered from the Internet Archive (wc3mods attachments 382/383) |
| `v2.35.4` | 2009-07-07 | Recovered from the Internet Archive (wc3mods attachments 457/458) |
| `v2.35.6` | 2011-03-29 | DA's final release (2.35.5 and 2.35.6 together), from forums.wc3mods.net attachments 485/486 |

Versions 0.2-1.3 predate the first public release. 2.0-2.35.3 and 2.35.5 were published only by
replacing the zips in the same forum posts, and no copies are known to survive. Their changes, and those of every release, are in
[CHANGELOG.md](CHANGELOG.md).

Later forks of 2.35.6 include [True-Games/UWC3NG](https://github.com/True-Games/UWC3NG) (2014) and
[heunetik/uwc3ng](https://github.com/heunetik/uwc3ng) (2.41.0, 2026). A "2.35.7" that circulates on
Polish sites is the 2.35.6 zip unchanged.

## Installation

From DA's installation guide:

1. Download a release and extract it into your mod folder (`cstrike/`), keeping the directory structure.
2. Add `uwc3ng.amxx` to the end of `addons/amxmodx/configs/plugins.ini`.
3. Edit `addons/amxmodx/configs/uwc3ng/uwc3ng.cfg`; every setting has a description.
4. Restart the server.

XP is saved to SQLite by default. Note that `uwc3ng_savexp_sql` is `0` for MySQL and `1` for SQLite.
Enable the matching module (`sqlite` or `mysql`) in `addons/amxmodx/configs/modules.ini`.

## Building

Every tagged version compiles unmodified with AMX Mod X 1.10 (warnings only):

    cd addons/amxmodx/scripting
    amxxpc uwc3ng.sma

## Credits

- **DA** - UWC3NG
- **Geesu** - Warcraft 3: Frozen Throne, which UWC3NG is based on
- **K2 (k2mia)** - the original UWC3
- **BAILOPAN and the AMX Mod X team**

## License

GNU General Public License v2 or later, with an exception for linking with the Half-Life engine and
Valve's mods. See the header of `uwc3ng.sma`.
