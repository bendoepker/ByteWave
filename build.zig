const std = @import("std");

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{
        .name = "ByteWave",
        .root_source_file = b.path("main.zig"),
        .target = b.host,
    });
    exe.addIncludePath(b.path("./include"));
    exe.addObjectFile(b.path("./lib/libportaudio.a"));

    b.installArtifact(exe);
}
