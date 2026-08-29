import { defineConfig } from "vite";
import react from "@vitejs/plugin-react-swc";
import { viteSingleFile } from "vite-plugin-singlefile";

// Single-file HTML (CSS/JS inlined). Relative base so canspec-data.json loads next to index.html
// at https://<org>.github.io/<repo>/canspec/
export default defineConfig({
  plugins: [react(), viteSingleFile()],
  base: process.env.VITE_BASE ?? "./",
  build: {
    outDir: "dist",
    emptyOutDir: true,
    assetsInlineLimit: 100000000,
    cssCodeSplit: false,
  },
});
