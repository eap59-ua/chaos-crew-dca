╔══════════════════════════════════════════════════════════════╗
║  CHAOS CREW - INSTRUCCIONES RÁPIDAS PARA ENTREGA FINAL      ║
║  Fecha límite: 12 ENERO 2026 - 23:59                        ║
╚══════════════════════════════════════════════════════════════╝

📋 ARCHIVOS IMPORTANTES:
- ENTREGA_FINAL.md → Instrucciones COMPLETAS paso a paso
- MENSAJE_PARA_COMPAÑERO.txt → Resumen para tu compañero
- generar_instalables.sh → Script Linux/WSL
- generar_instalables.bat → Script Windows

⚡ PASOS RÁPIDOS:

1️⃣ MERGEAR PR (OBLIGATORIO)
   https://github.com/eap59-ua/chaos-crew-dca/pulls
   → Mergear "fix/i18n-windows-support"

2️⃣ GENERAR INSTALABLES

   Windows (MSYS2):
   ----------------
   cd build_windows
   cmake --build . -j
   cpack -G ZIP
   → Genera: packages/chaos-crew-0.2.0-Windows.zip

   Linux/WSL:
   ----------
   cd build
   cmake --build . -j$(nproc)
   cpack -G DEB
   → Genera: packages/chaos-crew-0.2.0-Linux.deb

   O usar los scripts:
   - ./generar_instalables.sh (Linux/WSL)
   - generar_instalables.bat (Windows)

3️⃣ ITCH.IO (tu compañero)
   - Crear proyecto en itch.io
   - Subir los 2 instalables
   - Usar descripción de ENTREGA_FINAL.md
   - Copiar enlace final

4️⃣ PREPARAR ZIP PARA MOODLE
   Estructura:
   chaos-crew-entrega-final.zip
   ├── codigo-fuente/ (todo el repo desde main)
   ├── instalables/
   │   ├── chaos-crew-0.2.0-Windows.zip
   │   └── chaos-crew-0.2.0-Linux.deb
   └── ENLACE_ITCH_IO.txt

5️⃣ SUBIR A MOODLE
   - Solo UN miembro entrega
   - Incluir nombres de todo el equipo

═══════════════════════════════════════════════════════════════

🆘 SI HAY PROBLEMAS:
- Lee ENTREGA_FINAL.md con TODOS los detalles
- Comprueba que main está actualizado con el PR mergeado
- Verifica que los instalables funcionen antes de entregar

✅ TODO ESTÁ LISTO, SOLO EJECUTAR LOS PASOS!

═══════════════════════════════════════════════════════════════
