# Gobu

<p align="center">
  <a href="https://gobuengine.org">
    <img src="logo.png" width="200" alt="Godot Engine logo">
  </a>
</p>

**Gobu** es un software diseñado para crear juegos de manera intuitiva y eficiente, pensado para desarrolladores de todos los niveles, desde principiantes hasta expertos.

## Filosofía del proyecto
El objetivo principal de Gobu es **simplificar el desarrollo de videojuegos**, permitiendo que los desarrolladores se concentren en la creatividad y el diseño, sin perder tiempo en configuraciones complicadas o procesos innecesarios.

## Roadmap

### Parte 1 — Rendimiento

Uno de los objetivos principales de Gobu es mejorar radicalmente el **tiempo de carga del editor y de los proyectos**.  
Actualmente, Gobu carga el editor y un proyecto **hasta 1000% más rápido que Godot**, gracias a una reestructuración del core, inicialización diferida de sistemas y una arquitectura más ligera orientada al editor.

---

### Parte 2 — Filosofía de archivos

Gobu evita el uso de **escenas para todo**. Cada tipo de archivo tiene una responsabilidad clara:

- **Scene (`.scene`)**: unidades completas del proyecto (niveles, estados).
- **Prefab (`.prefab`)**: elementos reutilizables y componibles.
- **UIControl (`.uicontrol`)** *(en análisis)*: archivos dedicados exclusivamente a la UI.

Esta separación mejora la claridad del proyecto, la reutilización y el mantenimiento a largo plazo.
