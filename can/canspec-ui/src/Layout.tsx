import { useState } from "react";
import { NavLink, Outlet, useLocation } from "react-router-dom";
import { useCanspecData } from "./useCanspecData";

export function Layout() {
  const { data, error, loading } = useCanspecData();
  const [q, setQ] = useState("");
  const location = useLocation();
  const pathSegments = location.pathname.replace(/^\//, "").split("/").filter(Boolean);
  const showGlobalSearch = pathSegments.length < 2;

  if (loading) {
    return (
      <div className="spinner-wrap">
        <div className="spinner" aria-label="Loading" />
      </div>
    );
  }

  if (error || !data) {
    return (
      <div className="main">
        <p className="err">{error ?? "Unable to load CAN data."}</p>
      </div>
    );
  }

  return (
    <div className="layout-root">
      <header className="topbar">
        <div className="brand">
          <span className="brand-title">Embedded</span>
          <span className="brand-name">CAN Spec</span>
        </div>
        {showGlobalSearch ? (
          <label className="search-field">
            <span className="sr-only">Search</span>
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" aria-hidden>
              <path
                d="M10.5 18a7.5 7.5 0 110-15 7.5 7.5 0 010 15zM16.5 16.5L21 21"
                stroke="currentColor"
                strokeWidth="1.6"
                strokeLinecap="round"
              />
            </svg>
            <input
              type="search"
              placeholder="Filter vehicles and networks…"
              value={q}
              onChange={(e) => setQ(e.target.value)}
              autoComplete="off"
            />
          </label>
        ) : (
          <div className="topbar-spacer" aria-hidden />
        )}
        <div className="topbar-actions">
          <NavLink
            className={({ isActive }) =>
              `doc-link${isActive ? " doc-link--active" : ""}`
            }
            to="/"
            end
          >
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" aria-hidden>
              <path
                d="M19 17h2c.6 0 1-.4 1-1v-3c0-.9-.7-1.7-1.5-1.9C18.7 10.6 16 10 16 10s-1.3-1.5-2.2-2.3c-.5-.4-1.1-.7-1.8-.7H5c-.6 0-1.1.4-1.4.9l-1.4 2.9A3.7 3.7 0 002 12v4c0 .6.4 1 1 1h2"
                stroke="currentColor"
                strokeWidth="1.6"
                strokeLinecap="round"
                strokeLinejoin="round"
              />
              <circle cx="7.5" cy="17.5" r="2.5" stroke="currentColor" strokeWidth="1.6" />
              <circle cx="17.5" cy="17.5" r="2.5" stroke="currentColor" strokeWidth="1.6" />
            </svg>
            Vehicles
          </NavLink>
          <a className="doc-link" href="../">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" aria-hidden>
              <path
                d="M9 21H5a2 2 0 01-2-2V5a2 2 0 012-2h4M16 17l5-5-5-5M21 12H9"
                stroke="currentColor"
                strokeWidth="1.6"
                strokeLinecap="round"
                strokeLinejoin="round"
              />
            </svg>
            Documentation
          </a>
        </div>
      </header>
      <div className="app-shell">
        <main className="main main-full">
          <Outlet context={{ data, q, path: location.pathname }} />
        </main>
      </div>
    </div>
  );
}
