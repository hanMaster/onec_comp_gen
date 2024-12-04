import { create } from "zustand";

export interface Params {
  uuid: string;
  name: string;
  type: string;
}

interface Method {
  uuid: string;
  name: string;
  nameEng: string;
  params: Params[];
  hasReturn: boolean;
  ReturnType?: string;
  callRustMethod: boolean;
}

interface Props {
  uuid: string;
  name: string;
  canWrite: boolean;
  canRead: boolean;
}

interface State {
  methods: Method[];
  props: Props[];
  // actions
  addMethod: (method: Method) => void;
  saveMethod: (method: Method) => void;
  addProp: (prop: Props) => void;
  deleteMethod: (uuid: string) => void;
  deleteParam: (methodUuid: string, paramUuid: string) => void;
}

const useStore = create<State>()((set) => ({
  methods: [],
  props: [],
  addMethod: (method: Method) =>
    set((state) => ({ methods: [...state.methods, method] })),
  saveMethod: (method: Method) =>
    set((state) => ({
      methods: state.methods.map((m) => (m.uuid === method.uuid ? method : m)),
    })),
  addProp: (prop: Props) => set((state) => ({ props: [...state.props, prop] })),
  deleteMethod: (uuid: string) =>
    set((state) => ({ methods: state.methods.filter((m) => m.uuid !== uuid) })),
  deleteParam: (methodUuid: string, paramUuid: string) =>
    set((state) => ({
      methods: state.methods.map((method) =>
        method.uuid === methodUuid
          ? {
              ...method,
              params: method.params.filter((param) => param.uuid !== paramUuid),
            }
          : method
      ),
    })),
}));

export default useStore;
