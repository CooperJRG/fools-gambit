# Fool's Gambit — Master Build Plan

**Four parallel tracks. Tasks marked `[GATE: ...]` cannot start until the listed dependency is complete.**

- **Track E** — Engine Core (C++)
- **Track T** — Testing & Measurement
- **Track I** — Interface / Visualization (web)
- **Track D** — Documentation & Resume Polish

Each engine version is tagged when the corresponding `[E-vX.Y]` task completes. Site, model, and testing harness version independently.

---

## Phase 0 — Foundation (no gates)

### Track E — Engine Core
- [ ] **E-001** Set up CMake project, C++20/23
- [ ] **E-002** Add test framework (GoogleTest or Catch2)
- [ ] **E-003** Configure clang-format and clang-tidy
- [ ] **E-004** Stub `main.cpp` that launches and exits cleanly
- [ ] **E-005** Confirm `cmake --build .`, `ctest`, and `./foolsgambit` all run

### Track T — Testing & Measurement
- [ ] **T-001** Empty test passes in CTest
- [ ] **T-002** Create `tests/` directory structure (board, movegen, search, nnue, perft)

### Track D — Documentation
- [ ] **D-001** Initialize repo with `README.md` skeleton
- [ ] **D-002** Create `docs/roadmap.md`, `docs/engine-versions.md`, `docs/sprt-results.md`, `docs/nnue-notes.md`
- [ ] **D-003** Write project philosophy section in README

---

## Phase 1 — Board & Move Representation

### Track E
- [ ] **E-006** [GATE: E-005] Define `Square`, `Piece`, `Color`, `PieceType` enums
- [ ] **E-007** [GATE: E-006] `Bitboard` typedef (`uint64_t`) and helper ops (set/clear/test/popcount/lsb)
- [ ] **E-008** [GATE: E-007] `Board` struct: `pieces[2][6]`, `occupancy[2]`, `all_occupancy`, side-to-move, castling, ep, halfmove, fullmove
- [ ] **E-009** [GATE: E-008] Board printing / debug ASCII view
- [ ] **E-010** [GATE: E-008] FEN parser
- [ ] **E-011** [GATE: E-010] FEN serializer
- [ ] **E-012** [GATE: E-006] `Move` compact encoding (uint32_t, from/to/promo/flags)
- [ ] **E-013** [GATE: E-012] `move_to_uci` / `move_from_uci`

### Track T
- [ ] **T-003** [GATE: E-007] Bitboard unit tests (a1=bit 0, h8=bit 63, etc.)
- [ ] **T-004** [GATE: E-008] Board mutation tests (place/remove piece, occupancy stays consistent)
- [ ] **T-005** [GATE: E-011] FEN round-trip tests (startpos, Kiwipete, special cases)
- [ ] **T-006** [GATE: E-013] Move encode/decode tests, including promotion and castle flags

---

## Phase 2 — Move Generation

### Track E
- [ ] **E-014** [GATE: E-007] Precompute knight, king, pawn attack tables
- [ ] **E-015** [GATE: E-014] Ray-based bishop/rook/queen attacks (NOT magic yet)
- [ ] **E-016** [GATE: E-015] Pseudo-legal pawn moves (pushes, double pushes, captures, promotions, ep)
- [ ] **E-017** [GATE: E-014] Pseudo-legal knight/king moves
- [ ] **E-018** [GATE: E-015] Pseudo-legal bishop/rook/queen moves
- [ ] **E-019** [GATE: E-017] Castling move generation
- [ ] **E-020** [GATE: E-008, E-012] `make_move` with undo stack (captured piece, castling, ep, halfmove, zobrist slot, NNUE slot)
- [ ] **E-021** [GATE: E-020] `unmake_move`
- [ ] **E-022** [GATE: E-015] `is_square_attacked`, `in_check`
- [ ] **E-023** [GATE: E-021, E-022] Legal move filter (make → check own king → unmake)
- [ ] **E-024** [GATE: E-023] `perft(depth)` and `perft_divide`

### Track T
- [ ] **T-007** [GATE: E-014] Attack table tests (knight on d4 = 8 squares, etc.)
- [ ] **T-008** [GATE: E-015] Sliding attack tests with blockers (friendly stops, enemy capturable)
- [ ] **T-009** [GATE: E-021] Make/unmake exact-restore tests for all special moves
- [ ] **T-010** [GATE: E-022] Check detection tests (pins, double check, ep discovered check)
- [ ] **T-011** [GATE: E-024] **Perft startpos depths 1–6**
- [ ] **T-012** [GATE: E-024] **Perft Kiwipete** (depth 4+)
- [ ] **T-013** [GATE: E-024] Perft for castling, promotion, ep, pinned-piece positions

**🚧 HARD GATE: Do not proceed past Phase 2 until T-011, T-012, T-013 all pass.**

---

## Phase 3 — v0.1 UCI Random Engine

### Track E
- [ ] **E-025** [GATE: T-011, T-012] Implement UCI parser: `uci`, `isready`, `ucinewgame`, `position`, `go`, `stop`, `quit`
- [ ] **E-026** [GATE: E-025] UCI response: `id`, `uciok`, `readyok`, `bestmove`
- [ ] **E-027** [GATE: E-025] `go` selects uniformly random legal move
- [ ] **E-028** [GATE: E-027] UCI option `RandomSeed` for reproducibility
- [ ] **E-029** [GATE: E-028] **Tag v0.1-engine-random**

### Track T
- [ ] **T-014** [GATE: E-028] Engine loads in Cute Chess / Banksia / fastchess
- [ ] **T-015** [GATE: E-028] Engine plays a full game without crashing or producing illegal moves
- [ ] **T-016** [GATE: E-029] **SPRT harness scaffolding** in `testing/` (engines/, configs/, openings/, results/, scripts/)
- [ ] **T-017** [GATE: T-016] Install `cutechess-cli` or `fastchess`, install `ordo`/`bayeselo`
- [ ] **T-018** [GATE: T-017] Document SPRT command templates with bounds for early-version (`H0 +100, H1 +300`) and late-version (`H0 0, H1 +5`)

### Track D
- [ ] **D-004** [GATE: E-029] Add v0.1 row to `engine-versions.md`
- [ ] **D-005** [GATE: T-018] Document SPRT methodology in `docs/sprt-results.md`

---

## Phase 4 — Site v0.1 (Static Progress Page)

### Track I
- [ ] **I-001** [GATE: E-029] Initialize `site/` with React + TypeScript (Vite)
- [ ] **I-002** [GATE: I-001] Decide backend stack: FastAPI (recommended) or Express
- [ ] **I-003** [GATE: I-001] Static "project overview" page (`/`)
- [ ] **I-004** [GATE: I-001] `/progress` page with version timeline (manually maintained at first)
- [ ] **I-005** [GATE: I-001] `/docs` page linking to roadmap and version notes
- [ ] **I-006** [GATE: D-002] Render `docs/roadmap.md` and `docs/engine-versions.md` as MDX or fetched markdown
- [ ] **I-007** [GATE: I-004] Basic architecture diagram (SVG, mermaid, or excalidraw export)
- [ ] **I-008** [GATE: I-007] **Tag v0.1-site-static**

### Track D
- [ ] **D-006** [GATE: I-008] Link site to README

---

## Phase 5 — v0.2 Greedy Captures

### Track E
- [ ] **E-030** [GATE: E-029] Piece values constants (P=100, N=320, B=330, R=500, Q=900)
- [ ] **E-031** [GATE: E-030] MVV-style "capture most valuable victim" selector; fall back to random
- [ ] **E-032** [GATE: E-031] **Tag v0.2-engine-greedy**

### Track T
- [ ] **T-019** [GATE: E-032] Unit tests: hanging queen captured, rook preferred over pawn, no captures → random
- [ ] **T-020** [GATE: E-032, T-018] **SPRT v0.2 vs v0.1** (expect +200 to +400 Elo)
- [ ] **T-021** [GATE: T-020] Log result in `sprt-results.md`

### Track D
- [ ] **D-007** [GATE: T-021] Add v0.2 narrative to `engine-versions.md`

---

## Phase 6 — Site v0.2 (Playable Board)

### Track I
- [ ] **I-009** [GATE: I-008, E-029] Backend launches engine as UCI subprocess; manages stdin/stdout
- [ ] **I-010** [GATE: I-009] `POST /api/game/new` — creates game session
- [ ] **I-011** [GATE: I-009] `POST /api/game/move` — applies user move, queries engine, returns bestmove
- [ ] **I-012** [GATE: I-009] `POST /api/game/fen`, `GET /api/game/state`
- [ ] **I-013** [GATE: I-009] `GET /api/engine/info` — version, name, options
- [ ] **I-014** [GATE: I-010] Frontend chessboard component (react-chessboard or custom SVG)
- [ ] **I-015** [GATE: I-014] Legal move highlighting (request from backend)
- [ ] **I-016** [GATE: I-014] Move history panel + copy FEN button
- [ ] **I-017** [GATE: I-014] Play as white / black toggle, reset, load FEN
- [ ] **I-018** [GATE: I-017] **Tag v0.2-site-playable** — site can play against current engine

### Track T
- [ ] **T-022** [GATE: I-018] Smoke test: complete a full game through site against v0.2 engine

---

## Phase 7 — v0.3 Static Evaluation

### Track E
- [ ] **E-033** [GATE: E-032] Material evaluation function
- [ ] **E-034** [GATE: E-033] Piece-square tables (PeSTO-style; pick a clear sign convention and document it)
- [ ] **E-035** [GATE: E-034] Tapered evaluation (mg/eg interpolation)
- [ ] **E-036** [GATE: E-035] One-ply move selection: try each legal move, evaluate, pick best
- [ ] **E-037** [GATE: E-036] **Tag v0.3-engine-static-eval**

### Track T
- [ ] **T-023** [GATE: E-037] Eval unit tests (startpos ≈ 0, up a queen positive, central knight > rim knight)
- [ ] **T-024** [GATE: E-037] **SPRT v0.3 vs v0.2** (expect +200 to +400 Elo)
- [ ] **T-025** [GATE: T-024] Log SPRT result

### Track D
- [ ] **D-008** [GATE: T-025] Document evaluation design in `docs/`

---

## Phase 8 — v0.4 Negamax + v0.5 Alpha-Beta

### Track E
- [ ] **E-038** [GATE: E-037] Negamax with explicit checkmate/stalemate scoring (mate scores = `MATE - ply`)
- [ ] **E-039** [GATE: E-038] **Tag v0.4-engine-negamax**
- [ ] **E-040** [GATE: E-039] Alpha-beta pruning (fail-hard first; fail-soft later)
- [ ] **E-041** [GATE: E-040] **Tag v0.5-engine-alpha-beta**

### Track T
- [ ] **T-026** [GATE: E-039] Mate-in-one suite, stalemate not scored as mate, hanging queen captured at depth 2
- [ ] **T-027** [GATE: E-039] **SPRT v0.4 vs v0.3**
- [ ] **T-028** [GATE: E-041] At fixed depth: same best move as negamax, fewer nodes
- [ ] **T-029** [GATE: E-041] **SPRT v0.5 vs v0.4** (time-controlled, not fixed-depth)

---

## Phase 9 — Site v0.3 (Debug View)

### Track I
- [ ] **I-019** [GATE: I-018, E-041] Backend streams UCI `info` lines (WebSocket)
- [ ] **I-020** [GATE: I-019] Frontend "Search Info" panel: depth, nodes, nps, score, PV
- [ ] **I-021** [GATE: I-019] "Board State" panel: FEN, side to move, castling, ep, halfmove
- [ ] **I-022** [GATE: I-019] "Legal Moves" panel listing all legal moves
- [ ] **I-023** [GATE: I-019] "UCI Log" raw stream panel
- [ ] **I-024** [GATE: I-023] **Tag v0.3-site-debug**

---

## Phase 10 — v0.6 Quiescence + v0.7 Iterative Deepening

### Track E
- [ ] **E-042** [GATE: E-041] Quiescence search with stand-pat
- [ ] **E-043** [GATE: E-042] Capture ordering in qsearch (MVV-LVA initially)
- [ ] **E-044** [GATE: E-043] **Tag v0.6-engine-quiescence**
- [ ] **E-045** [GATE: E-044] Iterative deepening loop
- [ ] **E-046** [GATE: E-045] Principal variation tracking and `info pv ...` output
- [ ] **E-047** [GATE: E-045] Time management: `movetime`, `wtime/btime/winc/binc`, `stop`
- [ ] **E-048** [GATE: E-047] **Tag v0.7-engine-iterative**

### Track T
- [ ] **T-030** [GATE: E-044] Quiescence avoids horizon blunders; doesn't explode
- [ ] **T-031** [GATE: E-044] **SPRT v0.6 vs v0.5**
- [ ] **T-032** [GATE: E-048] Engine respects time controls; PV is legal
- [ ] **T-033** [GATE: E-048] **SPRT v0.7 vs v0.6**

---

## Phase 11 — Site v0.4 (Search Visualization)

### Track I
- [ ] **I-025** [GATE: I-024, E-048] Live PV display updating per iteration
- [ ] **I-026** [GATE: I-025] Eval-by-depth line chart (recharts)
- [ ] **I-027** [GATE: I-025] Best-move-changes-by-depth visualization
- [ ] **I-028** [GATE: I-027] **Tag v0.4-site-search-view**

---

## Phase 12 — v0.8 Zobrist + Transposition Table

### Track E
- [ ] **E-049** [GATE: E-048] Precompute Zobrist keys (pieces × squares, side, castling, ep file)
- [ ] **E-050** [GATE: E-049] Incremental hash update in make/unmake
- [ ] **E-051** [GATE: E-050] TT entry struct: key, depth, score, bound, best move, age
- [ ] **E-052** [GATE: E-051] TT probe / store with replacement scheme
- [ ] **E-053** [GATE: E-052] TT-aware mate score adjustment
- [ ] **E-054** [GATE: E-053] **Tag v0.8-engine-tt**

### Track T
- [ ] **T-034** [GATE: E-050] Incremental Zobrist = full recompute (random move sequences)
- [ ] **T-035** [GATE: E-050] Transposition test: same position via different move orders → same hash
- [ ] **T-036** [GATE: E-054] **SPRT v0.8 vs v0.7**

---

## Phase 13 — Database & Site v0.5 (SPRT Dashboard)

### Track I
- [ ] **I-029** [GATE: I-028] Add SQLite to backend; install ORM (SQLModel or Prisma)
- [ ] **I-030** [GATE: I-029] Schema: `engine_versions`, `matches`, `games`, `engine_outputs`
- [ ] **I-031** [GATE: I-030] Migration scripts + seed data for v0.1–v0.8
- [ ] **I-032** [GATE: I-030] `POST /api/matches` to ingest SPRT results (parse cutechess output)
- [ ] **I-033** [GATE: I-031] `/matches` page: list all version comparisons
- [ ] **I-034** [GATE: I-031] `/matches/:id` detail page: games, W/D/L, SPRT verdict, Elo gain, opening set, commit hash
- [ ] **I-035** [GATE: I-034] Estimated Elo timeline chart on `/progress`
- [ ] **I-036** [GATE: I-035] **Tag v0.5-site-sprt-dashboard**

### Track T
- [ ] **T-037** [GATE: I-032] SPRT runner script writes results to SQLite via API

### Track D
- [ ] **D-009** [GATE: I-036] Update README to feature the SPRT dashboard

---

## Phase 14 — v0.9 Move Ordering + Search Heuristics

### Track E
- [ ] **E-055** [GATE: E-054] Hash move first in ordering
- [ ] **E-056** [GATE: E-055] MVV-LVA capture ordering refined
- [ ] **E-057** [GATE: E-056] Killer move heuristic (2 slots per ply)
- [ ] **E-058** [GATE: E-057] History heuristic for quiets
- [ ] **E-059** [GATE: E-058] Countermove heuristic
- [ ] **E-060** [GATE: E-058] Principal Variation Search (PVS / NegaScout)
- [ ] **E-061** [GATE: E-060] Null move pruning (with zugzwang guard)
- [ ] **E-062** [GATE: E-060] Late move reductions (LMR)
- [ ] **E-063** [GATE: E-058] Static Exchange Evaluation (SEE)
- [ ] **E-064** [GATE: E-063] SEE-based bad capture pruning in qsearch
- [ ] **E-065** [GATE: E-062] Futility pruning, razoring, mate-distance pruning, check extensions
- [ ] **E-066** [GATE: E-065] UCI options to toggle each pruning feature
- [ ] **E-067** [GATE: E-066] **Tag v0.9-engine-heuristics**

### Track T
- [ ] **T-038** [GATE: E-055] **SPRT each feature independently** (E-057, E-058, E-060, E-061, E-062, E-063 each get their own SPRT)
- [ ] **T-039** [GATE: E-067] Perft unchanged after all changes
- [ ] **T-040** [GATE: E-067] Tactical suite (WAC, ECM) before/after comparison
- [ ] **T-041** [GATE: E-067] **SPRT v0.9 vs v0.8** (full bundle)

---

## Phase 15 — v0.10 Magic Bitboards + Bench

### Track E
- [ ] **E-068** [GATE: E-067] Generate (or use public) magic numbers for rooks and bishops
- [ ] **E-069** [GATE: E-068] Magic-based `bishop_attacks` / `rook_attacks`
- [ ] **E-070** [GATE: E-069] Swap ray-based sliding attacks for magic everywhere
- [ ] **E-071** [GATE: E-070] **Tag v0.10-engine-magic**
- [ ] **E-072** [GATE: E-067] `bench` command: fixed positions, fixed depth, deterministic node count
- [ ] **E-073** [GATE: E-072] Profile with `perf` / Instruments; document hotspots in `docs/profiling.md`

### Track T
- [ ] **T-042** [GATE: E-069] Magic attacks = ray attacks for millions of random blocker configs
- [ ] **T-043** [GATE: E-071] **SPRT v0.10 vs v0.9** (expect +50 to +150)
- [ ] **T-044** [GATE: E-072] Bench result regression tracking

---

## Phase 16 — Site v0.6 (Game Archive)

### Track I
- [ ] **I-037** [GATE: I-036] Store every SPRT game's PGN to DB
- [ ] **I-038** [GATE: I-037] `/games` browseable list, filter by version / result / opening
- [ ] **I-039** [GATE: I-038] `/games/:id` PGN replay with move-by-move board
- [ ] **I-040** [GATE: I-039] Eval graph per game (uses stored `engine_outputs`)
- [ ] **I-041** [GATE: I-040] Blunder detection (eval delta > 200cp) marked on graph
- [ ] **I-042** [GATE: I-041] **Tag v0.6-site-archive**

---

## Phase 17 — NNUE Foundation (v1.0 architecture)

### Track E
- [ ] **E-074** [GATE: E-071] Pick architecture: HalfKP or HalfKA, hidden size 256 or 512
- [ ] **E-075** [GATE: E-074] `feature_index(perspective, king_sq, piece_color, piece_type, piece_sq)` function
- [ ] **E-076** [GATE: E-075] Full accumulator computation from scratch
- [ ] **E-077** [GATE: E-076] Incremental accumulator update on make/unmake
- [ ] **E-078** [GATE: E-077] Dirty-piece tracking + king-move refresh logic
- [ ] **E-079** [GATE: E-078] Float inference path: accumulator → clipped ReLU → output → centipawns
- [ ] **E-080** [GATE: E-079] Weight file format (binary): magic, version, arch metadata, weights, biases, checksum
- [ ] **E-081** [GATE: E-080] Weight loader with version/corruption checks
- [ ] **E-082** [GATE: E-081] UCI option to switch between handcrafted eval and NNUE eval
- [ ] **E-083** [GATE: E-082] **Tag v1.0-engine-nnue-arch** (with dummy weights)

### Track T
- [ ] **T-045** [GATE: E-075] Feature index range + uniqueness tests; perspective tests
- [ ] **T-046** [GATE: E-077] **Incremental accumulator = full recompute** (thousands of random sequences) — *critical*
- [ ] **T-047** [GATE: E-077] Make → unmake restores accumulator exactly
- [ ] **T-048** [GATE: E-081] Save/load weights round-trip; corrupt file rejected
- [ ] **T-049** [GATE: E-083] **SPRT v1.0 vs v0.10** (expect -150 to +50; this version is about architecture, not strength)

### Track D
- [ ] **D-010** [GATE: E-074] Write `docs/nnue-notes.md`: explain accumulator in your own words

---

## Phase 18 — Data Pipeline & First Trained NNUE (v1.1)

### Track E
- [ ] **E-084** [GATE: E-083] Python: PGN → position extraction script
- [ ] **E-085** [GATE: E-084] Stockfish (or self-play) labeling pipeline → `(FEN, cp_score)` dataset
- [ ] **E-086** [GATE: E-085] Filtering: quiet positions, dedupe, drop extreme mate scores, skip opening
- [ ] **E-087** [GATE: E-086] PyTorch sparse-input NNUE training script (Huber or MSE loss)
- [ ] **E-088** [GATE: E-087] Weight export script (PyTorch → binary format from E-080)
- [ ] **E-089** [GATE: E-088] Train first real NNUE
- [ ] **E-090** [GATE: E-089] **Tag v1.1-engine-nnue-trained**, **model-v0.1**

### Track T
- [ ] **T-050** [GATE: E-086] Manually inspect 100 dataset samples for quality
- [ ] **T-051** [GATE: E-088] Python float output = C++ float output (parity test)
- [ ] **T-052** [GATE: E-090] **SPRT v1.1 vs v0.10** (expect +150 to +300)

### Track D
- [ ] **D-011** [GATE: E-089] Document training run: loss curves, MAE vs Stockfish, dataset size

---

## Phase 19 — Site v0.7 (NNUE / Model Dashboard)

### Track I
- [ ] **I-043** [GATE: I-042, E-090] Schema additions: `models` table (version, dataset, training metrics)
- [ ] **I-044** [GATE: I-043] `/models` page listing all trained NNUE versions
- [ ] **I-045** [GATE: I-044] `/models/:id` shows training loss curves, eval correlation, SPRT result
- [ ] **I-046** [GATE: I-045] Engine debug view shows NNUE eval, handcrafted eval, accumulator refresh count, dirty-feature count
- [ ] **I-047** [GATE: I-046] Parity status badge (Python/C++ float match status)
- [ ] **I-048** [GATE: I-047] **Tag v0.7-site-nnue**

---

## Phase 20 — Quantization, SIMD, v1.2

### Track E
- [ ] **E-091** [GATE: E-090] Quantization-aware training (int16 accumulator, int8 weights, int32 output)
- [ ] **E-092** [GATE: E-091] C++ integer inference path
- [ ] **E-093** [GATE: E-092] SSE2 baseline SIMD path
- [ ] **E-094** [GATE: E-093] AVX2 path with runtime CPU detection
- [ ] **E-095** [GATE: E-094] Cache alignment + struct layout pass
- [ ] **E-096** [GATE: E-095] **Tag v1.2-engine-quantized**

### Track T
- [ ] **T-053** [GATE: E-091] Float vs quantized output divergence within tolerance
- [ ] **T-054** [GATE: E-094] Bench: nps before/after each SIMD level
- [ ] **T-055** [GATE: E-096] **SPRT v1.2 vs v1.1** (expect +100 to +250)

---

## Phase 21 — v1.3 Maturity (Optional Advanced Features)

### Track E (pick at least 2)
- [ ] **E-097** [GATE: E-096] Polyglot opening book support
- [ ] **E-098** [GATE: E-096] Syzygy tablebase probing (Fathom)
- [ ] **E-099** [GATE: E-096] Aspiration windows
- [ ] **E-100** [GATE: E-096] Singular extensions
- [ ] **E-101** [GATE: E-096] ProbCut / multi-cut
- [ ] **E-102** [GATE: E-096] Reverse futility pruning, internal iterative reductions
- [ ] **E-103** [GATE: any 2 of E-097–E-102] **Tag v1.3-engine-mature**

### Track T
- [ ] **T-056** [GATE: each feature E-097–E-102] Individual SPRT per feature
- [ ] **T-057** [GATE: E-103] **SPRT v1.3 vs v1.2**

---

## Phase 22 — Resume Polish

### Track D
- [ ] **D-012** [GATE: T-052] Architecture diagram (engine subsystems + data flow)
- [ ] **D-013** [GATE: T-055] Write blog-style technical post per major feature in `docs/posts/`
- [ ] **D-014** [GATE: T-055] README hero section: Elo progression chart, screenshot of dashboard, SPRT methodology blurb
- [ ] **D-015** [GATE: T-055] Final resume bullets (drop into your master resume doc):
  - C++ UCI engine, bitboards, perft-verified movegen
  - Alpha-beta + iterative deepening + qsearch + TT + heuristics
  - SPRT-based Elo measurement across N versions, documented progression
  - HalfKP/HalfKA NNUE with incremental accumulator + quantization + AVX2 SIMD
  - Full-stack visualization: React/TS + FastAPI + SQLite, WebSocket live search streaming
  - Python training pipeline with C++/Python parity verification
- [ ] **D-016** [GATE: D-015] Publish GitHub release notes for each tagged version

### Track I
- [ ] **I-049** [GATE: D-014] Deploy site (Docker + Fly.io / Railway / your own box)
- [ ] **I-050** [GATE: I-049] Add to portfolio / personal site / resume link

---

## Permanent Regression Tests (run on every commit)

Run continuously in CI from Phase 2 onward:
- Perft startpos depths 1–5, Kiwipete, ep, castling, promotion positions
- Make/unmake exact-restore (random move sequences)
- Incremental Zobrist = full recompute (from Phase 12)
- Incremental NNUE accumulator = full recompute (from Phase 17)
- Bench node count stable (from Phase 15)
- C++/Python NNUE output parity (from Phase 18)
- Mate-in-N suite
- No illegal `bestmove` ever produced

---

## Critical Gate Summary

These are the hard gates — don't cheat past them:

| Gate | Blocks | Reason |
|------|--------|--------|
| **T-011, T-012, T-013** (perft) | Everything from Phase 3 onward | A buggy movegen poisons every SPRT result downstream |
| **T-018** (SPRT harness) | Every `[E-vX.Y]` tag from v0.2 forward | Untested versions = no resume story |
| **T-046** (NNUE accumulator parity) | All NNUE training work | The whole point of NNUE is incremental correctness |
| **T-051** (Python/C++ NNUE parity) | First trained model | Bugs here will silently waste weeks of training |

---

## Version Tagging Convention

Use independent versioning per track:
- `v0.X-engine-{feature}` — Track E milestones
- `v0.X-site-{feature}` — Track I milestones
- `model-v0.X` — Track E (NNUE) trained models
- `harness-v0.X` — Track T testing infrastructure milestones

Every engine tag must have a corresponding SPRT result before being considered "done."
