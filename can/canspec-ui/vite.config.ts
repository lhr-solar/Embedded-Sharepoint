import { defineConfig } from "vite";
import react from "@vitejs/plugin-react-swc";

// Relative base: assets and public files resolve next to index.html. Works for any host,
// project/repo name, or opening file:// from site/CAN-Spec/index.html after a local build.
// Override with VITE_BASE only if you need an absolute path (unusual).
export default defineConfig({
  plugins: [react()],
  base: process.env.VITE_BASE ?? "./",
  build: {
    outDir: "dist",
    emptyOutDir: true,
  },
});
